#include "Core.h"
#include "NanoVGModule.h"
#include "RenderModule.h"
#include "State.h"
#include <GL/glew.h>
#include <stdexcept>

namespace Cinnabar
{
	void RenderModule::init()
	{
		if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			throw std::runtime_error("SDL_Init: " + std::string(SDL_GetError()));
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		SDL_DisplayMode displayMode;
		SDL_GetDesktopDisplayMode(0, &displayMode);

		_window = SDL_CreateWindow(
			"Coin Wars 2019",
			0, 0,
			displayMode.w, displayMode.h,
			SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS
		);

		_glContext = SDL_GL_CreateContext(_window);

		//SDL_ShowCursor(false);

		SDL_GL_SetSwapInterval(1);

		glewExperimental = GL_TRUE;
		glewInit();
	}
	void RenderModule::shutdown()
	{
		SDL_GL_DeleteContext(_glContext);
		SDL_DestroyWindow(_window);
		SDL_Quit();
	}
	void RenderModule::update(float)
	{
		SDL_Event sdlEvent;
		while(SDL_PollEvent(&sdlEvent))
		{
			switch(sdlEvent.type)
			{
				case SDL_QUIT:
					core()->stop();
					break;
			}
			core()->eventBroker().emit(sdlEvent);
		}

		int width, height;
		SDL_GL_GetDrawableSize(_window, &width, &height);
		glViewport(0, 0, width, height);

		auto state = core()->state();
		if(state)
		{
			state->render();
		}

		if(auto nanovg = core()->module<NanoVGModule>())
		{
			nanovg->render();
		}

		SDL_GL_SwapWindow(_window);
	}
	Vector2 RenderModule::windowSize() const
	{
		int tmp[2];
		SDL_GL_GetDrawableSize(_window, &tmp[0], &tmp[1]);
		return Vector2(tmp[0], tmp[1]);
	}
}
