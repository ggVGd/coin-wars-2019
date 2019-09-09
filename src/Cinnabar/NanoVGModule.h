#pragma once
#include "Module.h"
#include "ThirdParty/nanovg/nanovg.h"

namespace Cinnabar
{
	class NanoVGModule : public Module<NanoVGModule>
	{
	public:
		void init() override;
		void shutdown() override;
		void update(float) override;

		void render();

		class Canvas;
		void setCanvas(Canvas*);

	private:
		NVGcontext* _ctx;
		Canvas* _canvas = nullptr;
	};

	class NanoVGModule::Canvas
	{
	public:
		virtual ~Canvas() = default;
		virtual void drawNanoVG() = 0;

	protected:
		NVGcontext* nvg() const;

	private:
		NVGcontext* _nvg;
		friend class NanoVGModule;
	};
}
