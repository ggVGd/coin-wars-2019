#pragma once
#include "Cinnabar/EventBroker.h"
#include "Cinnabar/Module.h"
#include "Enumerations.h"

class CoinControlModule
	: public Cinnabar::Module<CoinControlModule>,
	Cinnabar::EventBroker::Observer
{
public:
	void init() override;
	void shutdown() override;
	void update(float) override;

	void onEvent(const Cinnabar::EventBroker::Event*) override;
	void onEvent(const SDL_Event&);

private:
	void _emitCoinInsert(CoinType);
};
