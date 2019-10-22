#pragma once
#include "ASerial.h"
#include "Cinnabar/EventBroker.h"
#include "Cinnabar/Math/Vector2.h"
#include "Cinnabar/Module.h"
#include "Enumerations.h"
#include "Events.h"

class ScreenSaverModule
	: public Cinnabar::Module<ScreenSaverModule>,
	Cinnabar::EventBroker::Observer
{
public:
	void init() override;
	void shutdown() override;
	void update(float) override;

	void onEvent(const Cinnabar::EventBroker::Event*) override;
	void onEvent(const CoinInsertEvent&);

private:
	float _timer = 0.0f;
	bool _active = false;
	Cinnabar::Vector2i _lastMousePosition;

	void _sleep();
	void _wake();
};
