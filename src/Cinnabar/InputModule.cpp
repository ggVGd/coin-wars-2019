#include "InputModule.h"
#include "Core.h"
#include <algorithm>

namespace Cinnabar
{
	void InputModule::init()
	{
		core()->eventBroker().addObserver(this);
		subscribe<SDL_Event>();
	}
	void InputModule::shutdown()
	{
	}
	void InputModule::update(float)
	{
	}
	void InputModule::onEvent(const EventBroker::Event* event)
	{
		if(event->is<SDL_Event>())
		{
			onEvent(event->as<SDL_Event>());
		}
	}
	void InputModule::onEvent(const SDL_Event& event)
	{
		switch(event.type)
		{
			case SDL_WINDOWEVENT:
			{
				_window = SDL_GetWindowFromID(event.window.windowID);
				break;
			}
			case SDL_KEYDOWN:
			{
				if(!event.key.repeat)
				{
					keyPressed(event.key);
				}
				break;
			}
			case SDL_KEYUP:
			{
				if(!event.key.repeat)
				{
					keyReleased(event.key);
				}
				break;
			}
			case SDL_TEXTINPUT:
			{
				textInput(event.text);
				break;
			}
			case SDL_MOUSEMOTION:
			{
				mouseMoved(event.motion);
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				mousePressed(event.button);
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				mouseReleased(event.button);
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				mouseWheel(event.wheel);
				break;
			}
		}
	}
}
