#pragma once
#include "EventBroker.h"
#include "Module.h"
#include "Signals.h"

namespace Cinnabar
{
	class InputModule : public Module<InputModule>, EventBroker::Observer
	{
	public:
		void init() override;
		void shutdown() override;
		void update(float) override;
		
		void onEvent(const EventBroker::Event*) override;
		void onEvent(const SDL_Event&);

		Signal<const SDL_KeyboardEvent&>    keyPressed;
		Signal<const SDL_KeyboardEvent&>    keyReleased;
		Signal<const SDL_TextInputEvent&>   textInput;
		Signal<const SDL_MouseMotionEvent&> mouseMoved;
		Signal<const SDL_MouseButtonEvent&> mousePressed;
		Signal<const SDL_MouseButtonEvent&> mouseReleased;
		Signal<const SDL_MouseWheelEvent&>  mouseWheel;

	private:
		SDL_Window* _window;
	};
}
