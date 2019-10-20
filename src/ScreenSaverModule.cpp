#include "ScreenSaverModule.h"
#include "Cinnabar/Core.h"
#include "Events.h"
#include "Settings.h"
#include "VideoControlModule.h"

void ScreenSaverModule::init()
{
}
void ScreenSaverModule::shutdown()
{
}
void ScreenSaverModule::update(float elapsed)
{
	auto videoControl = core()->module<VideoControlModule>();
	const float screenSaverTime = Settings::get<float>("screenSaverTime");

	Cinnabar::Vector2i mousePosition;
	SDL_GetGlobalMouseState(&mousePosition.x, &mousePosition.y);

	if(mousePosition != _lastMousePosition)
	{
		_lastMousePosition = mousePosition;
		_timer = 0.0f;
		if(_active && videoControl->isPlayerOpen())
		{
			core()->eventBroker().emit(VideoStopRequestEvent());
			core()->eventBroker().emit(DepartmentDeselectEvent());
			_active = false;
		}
	}
	else
	{
		_timer += elapsed;
		if(_timer >= screenSaverTime && !videoControl->isPlayerOpen())
		{
			core()->eventBroker().emit(VideoPlayRequestEvent{
				"resources/video/promo.mp4",
				true
			});
			_active = true;
		}
	}
}
