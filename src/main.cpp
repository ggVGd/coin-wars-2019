#include "Cinnabar/Core.h"
#include "Cinnabar/InputModule.h"
#include "Cinnabar/NanoVGModule.h"
#include "Cinnabar/RenderModule.h"
#include "CoinControlModule.h"
#include "MainState.h"
#include "ScreenSaverModule.h"
#include "VideoControlModule.h"

int main(int argc, char** argv)
{
	Cinnabar::Core core;
	core.addModule<Cinnabar::RenderModule>();
	core.addModule<Cinnabar::InputModule>();
	core.addModule<Cinnabar::NanoVGModule>();
	core.addModule<CoinControlModule>();
	core.addModule<ScreenSaverModule>();
	core.addModule<VideoControlModule>();

	core.init();

	core.pushState<MainState>();

	core.run();

	core.shutdown();
	return 0;
}
