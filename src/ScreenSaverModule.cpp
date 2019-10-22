#include "ScreenSaverModule.h"
#include "Cinnabar/Core.h"
#include "Events.h"
#include "Settings.h"
#include "VideoControlModule.h"

void ScreenSaverModule::init()
{
	core()->eventBroker().addObserver(this);
	subscribe<CoinInsertEvent>();
}
void ScreenSaverModule::shutdown()
{
}
void ScreenSaverModule::update(float elapsed)
{
	const float screenSaverTime = Settings::get<float>("screenSaverTime");

	Cinnabar::Vector2i mousePosition;
	SDL_GetGlobalMouseState(&mousePosition.x, &mousePosition.y);

	if(mousePosition != _lastMousePosition)
	{
		_lastMousePosition = mousePosition;
		_wake();
	}
	else
	{
		_timer += elapsed;
		if(_timer >= screenSaverTime)
			_sleep();
	}
}
void ScreenSaverModule::onEvent(const Cinnabar::EventBroker::Event* event)
{
	if(event->is<CoinInsertEvent>())
	{
		onEvent(event->as<CoinInsertEvent>());
	}
}
void ScreenSaverModule::onEvent(const CoinInsertEvent& event)
{
	_wake();
}
void ScreenSaverModule::_sleep()
{
	auto videoControl = core()->module<VideoControlModule>();
	if(!videoControl->isPlayerOpen())
	{
		core()->eventBroker().emit(VideoPlayRequestEvent{
			"resources/video/promo.mp4",
			true
		});
		_active = true;
	}
}
void ScreenSaverModule::_wake()
{
	_timer = 0.0f;
	auto videoControl = core()->module<VideoControlModule>();
	if(_active && videoControl->isPlayerOpen())
	{
		core()->eventBroker().emit(VideoStopRequestEvent());
		core()->eventBroker().emit(DepartmentDeselectEvent());
		_active = false;
	}
}
