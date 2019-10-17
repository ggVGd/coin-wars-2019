#include "CoinControlModule.h"
#include "Cinnabar/Core.h"
#include "Events.h"

void CoinControlModule::init()
{
	core()->eventBroker().addObserver(this);
	subscribe<SDL_Event>();
	serialPort = new ASerial("/dev/ttyACM0");
}
void CoinControlModule::shutdown()
{
	delete serialPort;
}
void CoinControlModule::update(float)
{
	CoinType coin = CoinType::Invalid;
	while(serialPort->canRead())
	{
		coin = serialPort->getCommand();
		printf("coin=%d\n", (int)coin);
		if(coin != CoinType::Invalid)
			_emitCoinInsert(coin);
	}
}
void CoinControlModule::onEvent(const Cinnabar::EventBroker::Event* event)
{
	if(event->is<SDL_Event>())
	{
		onEvent(event->as<SDL_Event>());
	}
}
void CoinControlModule::onEvent(const SDL_Event& event)
{
	switch(event.type)
	{
		case SDL_KEYDOWN:
		{
			if(event.key.repeat)
				break;
			if(event.key.keysym.sym == SDLK_1)
				_emitCoinInsert(CoinType::Penny);
			if(event.key.keysym.sym == SDLK_2)
				_emitCoinInsert(CoinType::Nickel);
			if(event.key.keysym.sym == SDLK_3)
				_emitCoinInsert(CoinType::Dime);
			if(event.key.keysym.sym == SDLK_4)
				_emitCoinInsert(CoinType::Quarter);
			break;
		}
	}
}
void CoinControlModule::_emitCoinInsert(CoinType coin)
{
	core()->eventBroker().emit(CoinInsertEvent{ coin });
}
