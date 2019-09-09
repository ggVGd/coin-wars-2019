#pragma once
#include "Math/Vector2.h"
#include "Module.h"

namespace Cinnabar
{
	class RenderModule : public Module<RenderModule>
	{
	public:
		void init() override;
		void shutdown() override;
		void update(float) override;

		Vector2 windowSize() const;

	private:
		SDL_Window* _window;
		SDL_GLContext _glContext;
	};
}
