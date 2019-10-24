#include "Board.h"
#include "Cinnabar/Buffer.h"
#include "Cinnabar/Core.h"
#include "Cinnabar/InputModule.h"
#include "Cinnabar/Math/Math.h"
#include "Cinnabar/Math/Random.h"
#include "Cinnabar/RenderModule.h"
#include "Cinnabar/ShaderProgram.h"
#include "Cinnabar/Sprite.h"
#include "Cinnabar/Texture.h"
#include "Cinnabar/VertexArray.h"
#include "Cinnabar/VertexBuffer.h"
#include "Enumerations.h"
#include "Events.h"
#include "Settings.h"
#include <GL/gl.h>

#define PHYSICS_SCALE         0.01f
#define PUCK_RADIUS           Settings::get<float>("puck_radius")
#define PLACEMENT_AREA_HEIGHT Settings::get<float>("placement_area_height")

struct BodyData
{
	std::shared_ptr<Cinnabar::Sprite> sprite = nullptr;
	float bucketTimer = 0.0f;
	CoinType coinType;
};

Board::Board(const Cinnabar::Vector2& size)
	: _size(size), _physicsWorld(b2Vec2(0.0f, -10.0f))
{
	_sceneRenderer.setScene(&_scene);

	_loadResources();
	_setupBackground();
	_setupBoard();

	_placingPuckSprite = std::make_shared<Cinnabar::Sprite>();
	_placingPuckSprite->setSize(PUCK_RADIUS * 2.0f, PUCK_RADIUS * 2.0f);
	_placingPuckSprite->setTexture(_puckTextures[0]);
	_placingPuckSprite->setPosition(-1000, -1000);
	_scene.sprites.push_back(_placingPuckSprite);

	Cinnabar::Core::getSingleton()->eventBroker().addObserver(this);
	subscribe<DepartmentSelectEvent>();
	subscribe<DepartmentDeselectEvent>();
}
void Board::onEvent(const Cinnabar::EventBroker::Event* event)
{
	if(event->is<DepartmentSelectEvent>())
	{
		onEvent(event->as<DepartmentSelectEvent>());
	}
	else if(event->is<DepartmentDeselectEvent>())
	{
		onEvent(event->as<DepartmentDeselectEvent>());
	}
}
void Board::onEvent(const DepartmentSelectEvent& event)
{
	_department = event.department;
}
void Board::onEvent(const DepartmentDeselectEvent& event)
{
	_department.clear();
}
void Board::render(const glm::mat4& MVP)
{
	_syncSprites();
	_sceneRenderer.render(MVP);
}
void Board::update(float elapsed)
{
	if(_department.empty())
		return;

	_physicsWorld.Step(elapsed, 8, 3);

	const float bucket_area_height = Settings::get<float>("bucket_area_height") * PHYSICS_SCALE;
	const float bucket_height = Settings::get<float>("bucket_height");
	const float bucket_width = _size.x / Settings::bucketSizeFactors();
	const float bucket_time = Settings::get<float>("bucket_time");

	std::vector<b2Body*> bodiesToDelete;
	for(auto body = _physicsWorld.GetBodyList(); body; body = body->GetNext())
	{
		if(body->GetMass() == 0.0f)
			continue;

		if(!body->IsAwake())
		{
			body->ApplyForce(
				b2Vec2(Cinnabar::Math::randSymmetric(), 0.0f),
				b2Vec2(0, 0),
				true
			);
		}

		if(body->GetPosition().y < bucket_height * PHYSICS_SCALE)
		{
			auto bodyData = static_cast<BodyData*>(body->GetUserData());
			bodyData->bucketTimer += elapsed;
			if(bodyData->bucketTimer >= bucket_time)
			{
				bodiesToDelete.push_back(body);
			}
		}
	}

	for(auto body : bodiesToDelete)
	{
		auto bodyData = static_cast<BodyData*>(body->GetUserData());
		if(bodyData)
		{
			if(bodyData->sprite)
			{
				_scene.removeSprite(bodyData->sprite);
			}

			const auto position = Cinnabar::Vector2(body->GetPosition().x, body->GetPosition().y) / PHYSICS_SCALE;
			const auto bucketSlot = (int)floor(position.x / bucket_width);
			const auto bucketIndex = Settings::bucketIndex(bucketSlot);

			Cinnabar::Core::getSingleton()->eventBroker().emit(PuckBucketEvent{
				bucketIndex,
				bodyData->coinType,
				position
			});

			delete bodyData;
		}
		_physicsWorld.DestroyBody(body);
	}
}

void Board::beginPlace(CoinType coinType)
{
	_placing = true;
	_placingCoinType = coinType;
	_placingPuckSprite->setTexture(_puckTextures[static_cast<std::underlying_type<CoinType>::type>(coinType)]);
}
void Board::setPlacingPosition(glm::vec2 position)
{
	if(_placing)
	{
		position.y = Cinnabar::clamp(position.y, _size.y - PLACEMENT_AREA_HEIGHT, _size.y);
		_placingPuckSprite->setPosition(position);
	}
}
void Board::endPlace()
{
	if(_placing)
	{
		_placing = false;
		auto body = _createPuck(
			_placingPuckSprite->position().x + PUCK_RADIUS,
			_placingPuckSprite->position().y + PUCK_RADIUS,
			PUCK_RADIUS,
			_placingCoinType
		);
		auto bodyData = static_cast<BodyData*>(body->GetUserData());
		bodyData->coinType = _placingCoinType;
		_placingPuckSprite->setPosition(-1000, -1000);
	}
}

void Board::_loadResources()
{
	_backgroundTexture = std::make_shared<Cinnabar::Texture>();
	_backgroundTexture->load("resources/textures/background.png");

	_pegTexture = std::make_shared<Cinnabar::Texture>();
	_pegTexture->load("resources/textures/peg.png");

	_puckTextures.resize(5);
	for(int i = 0; i < 5; i++)
	{
		_puckTextures[i] = std::make_shared<Cinnabar::Texture>();
		_puckTextures[i]->load("resources/textures/puck_" + std::to_string(i) + ".png");
	}

	_wallTexture = std::make_shared<Cinnabar::Texture>();
	_wallTexture->load("resources/textures/wall.png");
}
void Board::_setupBackground()
{
	auto sprite = std::make_shared<Cinnabar::Sprite>();
	sprite->setSize(_size.x, _size.y);
	sprite->setTexture(_backgroundTexture);
	sprite->setZIndex(-100);
	_scene.sprites.push_back(sprite);
}
b2Body* Board::_createPuck(float x, float y, float radius, CoinType coinType)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x * PHYSICS_SCALE, y * PHYSICS_SCALE);
	auto body = _physicsWorld.CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = radius * PHYSICS_SCALE;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = Settings::get<float>("puck_density");
	fixtureDef.friction = Settings::get<float>("puck_friction");
	fixtureDef.restitution = Settings::get<float>("puck_restitution");
	body->CreateFixture(&fixtureDef);

	auto sprite = std::make_shared<Cinnabar::Sprite>();
	sprite->setSize(radius * 2.0f, radius * 2.0f);
	sprite->setZIndex(100);
	sprite->setTexture(_puckTextures[static_cast<std::underlying_type<CoinType>::type>(coinType)]);
	_scene.sprites.push_back(sprite);

	auto bodyData = new BodyData;
	bodyData->sprite = sprite;
	body->SetUserData(bodyData);

	return body;
}
void Board::_createPeg(float x, float y, float radius)
{
	b2BodyDef bodyDef;
	bodyDef.position.Set(x * PHYSICS_SCALE, y * PHYSICS_SCALE);
	auto body = _physicsWorld.CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = radius * PHYSICS_SCALE;
	body->CreateFixture(&shape, 0.0f);

	auto sprite = std::make_shared<Cinnabar::Sprite>();
	sprite->setTexture(_pegTexture);
	sprite->setSize(radius * 2.0f, radius * 2.0f);
	sprite->setPosition(x - radius * 0.5f, y - radius * 0.5f);
	sprite->setZIndex(100);
	_scene.sprites.push_back(sprite);

	auto bodyData = new BodyData;
	bodyData->sprite = sprite;
	body->SetUserData(bodyData);
}
void Board::_createWall(float x1, float y1, float x2, float y2, bool withSprite)
{
	b2BodyDef bodyDef;
	bodyDef.position.Set(0, 0);
	auto body = _physicsWorld.CreateBody(&bodyDef);

	b2EdgeShape shape;
	shape.Set(
		b2Vec2(x1 * PHYSICS_SCALE, y1 * PHYSICS_SCALE),
		b2Vec2(x2 * PHYSICS_SCALE, y2 * PHYSICS_SCALE)
	);
	body->CreateFixture(&shape, 0.0f);

	if(withSprite)
	{
		const float length = (Cinnabar::Vector2(x1, y1) - Cinnabar::Vector2(x2, y2)).length();

		auto sprite = std::make_shared<Cinnabar::Sprite>();
		sprite->setTexture(_wallTexture);
		sprite->setSize(length, 10.0f);
		sprite->setPosition((x1 + x2) * 0.5f, (y1 + y2) * 0.5f);
		sprite->setCenteredPosition(true);
		sprite->setZIndex(50);
	
		const auto v = Cinnabar::Vector2(x2 - x1, y2 - y1).normalizedCopy();
		const float angle = atan2(v.y, v.x) - atan2(0, 1);
		sprite->setRotation(angle);

		_scene.sprites.push_back(sprite);
	}
}
void Board::_setupBoard()
{
	const float peg_area_top    = _size.y - Settings::get<float>("peg_area_top");
	const float peg_area_bottom = Settings::get<float>("peg_area_bottom");
	const float peg_spacing_x   = Settings::get<float>("peg_spacing_x");
	const float peg_spacing_y   = Settings::get<float>("peg_spacing_y");
	const int width             = floor(_size.x / peg_spacing_x);
	const float margin_x        = fmod(_size.x, peg_spacing_x) * 0.5f;

	float previousWallPeg[4];

	auto isLastCol = [&](int row, int col){
		if(row % 2 == 0)
			return col == width;
		return col == width - 1;
	};

	int row = 0, col = 0;
	for(int y = peg_area_bottom; y <= peg_area_top; row++, y += peg_spacing_y)
	{
		for(col = 0; !isLastCol(row, col - 1); col++)
		{
			const float x = margin_x + col * peg_spacing_x + (row % 2) * peg_spacing_x * 0.5f;
			_createPeg(x, y, Settings::get<float>("peg_radius"));

			if(row > 0 && col == 0)
			{
				_createWall(previousWallPeg[0], previousWallPeg[1], x, y);
			}
			else if(row > 0 && isLastCol(row, col))
			{
				_createWall(previousWallPeg[2], previousWallPeg[3], x, y);
			}
			if(col == 0)
			{
				previousWallPeg[0] = x;
				previousWallPeg[1] = y;
			}
			else if(isLastCol(row, col))
			{
				previousWallPeg[2] = x;
				previousWallPeg[3] = y;
			}
		}
	}

	if(row % 2 == 0)
	{
		const float x = margin_x + col * peg_spacing_x + peg_spacing_x * 0.5f;
		const float y = peg_area_bottom + peg_spacing_y * (floor((peg_area_top - peg_area_bottom) / peg_spacing_y) + 1);
		_createWall(previousWallPeg[0], previousWallPeg[1], 0, y);
		_createWall(previousWallPeg[2], previousWallPeg[3], x, y);
	}

	_createWall(0, 0, 0, _size.y, false);
	_createWall(0, 0, _size.x, 0, false);
	_createWall(_size.x, 0, _size.x, _size.y, false);
	
	const float bucket_height = Settings::get<float>("bucket_height");
	const float bucket_width = _size.x / Settings::bucketSizeFactors();
	float x0 = 0, x1 = 0;
	for(int i = 0; i < Settings::buckets().size() - 1; i++)
	{
		const auto& bucket = Settings::buckets().at(i);
		x1 = x0 + bucket_width * bucket.sizeFactor;
		_createWall(x1, 0, x1, bucket_height);
		x0 = x1;
	}
}
void Board::_syncSprites()
{
	for(auto body = _physicsWorld.GetBodyList(); body; body = body->GetNext())
	{
		if(!body->GetUserData())
			continue;
		auto bodyData = static_cast<BodyData*>(body->GetUserData());
		if(!bodyData->sprite)
			continue;
		bodyData->sprite->setPosition(
			body->GetPosition().x / PHYSICS_SCALE - bodyData->sprite->size().x * 0.5f,
			body->GetPosition().y / PHYSICS_SCALE - bodyData->sprite->size().y * 0.5f
		);
		bodyData->sprite->setRotation(body->GetAngle());
	}
}
