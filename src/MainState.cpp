#include "MainState.h"
#include "Cinnabar/Core.h"
#include "Cinnabar/Sprite.h"
#include "Cinnabar/RenderModule.h"
#include "Cinnabar/InputModule.h"
#include "Cinnabar/Texture.h"
#include "Events.h"
#include "Settings.h"
#include <GL/gl.h>

#define PUCK_RADIUS Settings::get<float>("puck_radius")

MainState::MainState()
{
}
void MainState::enter()
{
	core()->eventBroker().addObserver(this);
	subscribe<CoinInsertEvent>();
	subscribe<PuckBucketEvent>();
	subscribe<VideoEndEvent>();

	auto render = core()->module<Cinnabar::RenderModule>();
	_board = new Board(render->windowSize(), core()->eventBroker());

	auto input = core()->module<Cinnabar::InputModule>();
	input->mousePressed.subscribe(this, std::bind(&MainState::mousePressed, this, std::placeholders::_1));
	input->mouseReleased.subscribe(this, std::bind(&MainState::mouseReleased, this, std::placeholders::_1));
	input->mouseMoved.subscribe(this, std::bind(&MainState::mouseMoved, this, std::placeholders::_1));

	auto nanovg = core()->module<Cinnabar::NanoVGModule>();
	nanovg->setCanvas(this);
}
void MainState::leave()
{
	core()->eventBroker().removeObserver(this);

	auto input = core()->module<Cinnabar::InputModule>();
	input->mousePressed.unsubscribe(this);
	input->mouseReleased.unsubscribe(this);
	input->mouseMoved.unsubscribe(this);

	auto nanovg = core()->module<Cinnabar::NanoVGModule>();
	nanovg->setCanvas(nullptr);
}
void MainState::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	auto renderModule = core()->module<Cinnabar::RenderModule>();

	glm::mat4 projection = glm::ortho(
		0.0f, (float)renderModule->windowSize().x,
		0.0f, (float)renderModule->windowSize().y,
		0.0f, 100.0f
	);
	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	glm::mat4 VP = projection * view;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_board->render(VP);
}
void MainState::update(float elapsed)
{
	if(!_showingResult)
	{
		_board->update(elapsed);
	}
}
void MainState::drawNanoVG()
{
	//nvgBeginPath(nvg());
	//nvgRect(nvg(), 100, 100, 200, 200);
	//nvgFillColor(nvg(), nvgRGBA(255, 255, 255, 105));
	//nvgFill(nvg());

	nvgFontSize(nvg(), 20.0f);
	nvgFontFaceId(nvg(), nvgFindFont(nvg(), "OpenSans-Regular"));
	nvgTextAlign(nvg(), NVG_ALIGN_LEFT);
	nvgFontBlur(nvg(), 0);
	nvgFillColor(nvg(), nvgRGBA(255, 255, 255, 255));

	int y = 0;
	for(const auto& coin : _coinQueue)
	{
		char text[16];
		sprintf(text, "Coin %d", static_cast<uint8_t>(coin));
		nvgText(nvg(), 10, 10 + y, text, nullptr);
		y += 20;
	}
}
void MainState::onEvent(const Cinnabar::EventBroker::Event* event)
{
	if(event->is<CoinInsertEvent>())
	{
		onEvent(event->as<CoinInsertEvent>());
	}
	else if(event->is<PuckBucketEvent>())
	{
		onEvent(event->as<PuckBucketEvent>());
	}
	else if(event->is<VideoEndEvent>())
	{
		onEvent(event->as<VideoEndEvent>());
	}
}
void MainState::onEvent(const CoinInsertEvent& event)
{
	_coinQueue.push_back(event.coinType);
}
void MainState::onEvent(const PuckBucketEvent& event)
{
	_showingResult = true;

	core()->eventBroker().emit(VideoPlayRequestEvent{
		"resources/video/rickroll.mp4",
		false
	});
}
void MainState::onEvent(const VideoEndEvent& event)
{
	_showingResult = false;
}
void MainState::mousePressed(const SDL_MouseButtonEvent& event)
{
	if(!_coinQueue.empty())
	{
		_placing = true;
		auto coinType = _coinQueue.front();
		_coinQueue.pop_front();

		auto renderModule = core()->module<Cinnabar::RenderModule>();
		const auto windowH = renderModule->windowSize().y;
		_board->beginPlace(coinType);
		_board->setPlacingPosition({
			event.x - PUCK_RADIUS,
			windowH - event.y - PUCK_RADIUS
		});
	}
}
void MainState::mouseReleased(const SDL_MouseButtonEvent& event)
{
	_board->endPlace();
}
void MainState::mouseMoved(const SDL_MouseMotionEvent& event)
{
	auto renderModule = core()->module<Cinnabar::RenderModule>();
	const auto windowH = renderModule->windowSize().y;
	_board->setPlacingPosition({
		event.x - PUCK_RADIUS,
		windowH - event.y - PUCK_RADIUS
	});
}
