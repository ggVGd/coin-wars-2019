#pragma once
#include "Cinnabar/EventBroker.h"
#include "Cinnabar/Math/Vector2.h"
#include "Cinnabar/Module.h"
#include "Enumerations.h"
#include "ASerial.h"

class ScreenSaverModule : public Cinnabar::Module<ScreenSaverModule>
{
public:
	void init() override;
	void shutdown() override;
	void update(float) override;

private:
	float _timer = 0.0f;
	bool _active = false;
	Cinnabar::Vector2i _lastMousePosition;
};
