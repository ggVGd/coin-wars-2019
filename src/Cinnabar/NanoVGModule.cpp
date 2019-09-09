#include "Core.h"
#include "NanoVGModule.h"
#include "RenderModule.h"
#include <GL/glew.h>
#include <boost/filesystem.hpp>

#define NANOVG_GLES2_IMPLEMENTATION
#include "ThirdParty/nanovg/nanovg_gl.h"

namespace Cinnabar
{
	void NanoVGModule::init()
	{
		_ctx = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

		using namespace boost::filesystem;
		path dir("./resources/fonts");
		for(directory_iterator it(dir); it != directory_iterator(); it++)
		{
			if(it->path().extension() != ".ttf")
				continue;

			nvgCreateFont(_ctx,
				it->path().stem().c_str(),
				it->path().c_str()
			);
		}
	}
	void NanoVGModule::shutdown()
	{
		nvgDeleteGLES2(_ctx);
	}
	void NanoVGModule::update(float)
	{
	}
	void NanoVGModule::render()
	{
		if(!_canvas)
			return;

		auto render = core()->module<RenderModule>();
		const auto& windowSize = render->windowSize();

		nvgBeginFrame(_ctx, windowSize.x, windowSize.y, 1);
		nvgShapeAntiAlias(_ctx, true);

		_canvas->drawNanoVG();

		nvgEndFrame(_ctx);
	}
	void NanoVGModule::setCanvas(Canvas* canvas)
	{
		_canvas = canvas;
		_canvas->_nvg = _ctx;
	}


	NVGcontext* NanoVGModule::Canvas::nvg() const
	{
		return _nvg;
	}
}
