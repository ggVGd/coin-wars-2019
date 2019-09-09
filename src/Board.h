#pragma once
#include "Cinnabar/EventBroker.h"
#include "Enumerations.h"
#include "Scene.h"
#include "SceneRenderer.h"
#include <Box2D/Box2D.h>
#include <SDL2/SDL_events.h>

class Board
{
public:
	Board(const Cinnabar::Vector2& size, Cinnabar::EventBroker&);
	void bind();
	void unbind();

	void render(const glm::mat4& MVP);
	void update(float);

	void beginPlace(CoinType);
	void setPlacingPosition(glm::vec2);
	void endPlace();

private:
	const Cinnabar::Vector2 _size;
	Cinnabar::EventBroker& _eventBroker;
	b2World _physicsWorld;

	Scene _scene;
	SceneRenderer _sceneRenderer;

	std::shared_ptr<Cinnabar::Texture> _backgroundTexture;
	std::shared_ptr<Cinnabar::Texture> _pegTexture;
	std::shared_ptr<Cinnabar::Texture> _puckTexture;
	std::shared_ptr<Cinnabar::Texture> _wallTexture;

	bool _placing = false;
	CoinType _placingCoinType;
	std::shared_ptr<Cinnabar::Sprite> _placingPuckSprite;

	void _loadResources();
	void _setupBackground();
	b2Body* _createPuck(float x, float y, float radius);
	void _createPeg(float x, float y, float radius);
	void _createWall(float x1, float y1, float x2, float y2, bool withSprite = true);
	void _setupBoard();
	void _syncSprites();
};
