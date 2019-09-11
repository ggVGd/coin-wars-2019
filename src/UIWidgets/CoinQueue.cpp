#include "CoinQueue.h"

namespace UIWidgets
{
	void CoinQueue::bind(Cinnabar::EventBroker& eventBroker)
	{
		eventBroker.addObserver(this);
		subscribe<CoinInsertEvent>();
		subscribe<CoinConsumeEvent>();
	}
	void CoinQueue::render()
	{
		nvgFontSize(ctx(), 20.0f);
		nvgFontFaceId(ctx(), nvgFindFont(ctx(), "OpenSans-Regular"));
		nvgTextAlign(ctx(), NVG_ALIGN_LEFT);
		nvgFontBlur(ctx(), 0);
		nvgFillColor(ctx(), nvgRGBA(255, 255, 255, 255));

		int y = 0;
		for(const auto& coin : _coinQueue)
		{
			switch(coin)
			{
				case CoinType::Penny:
					nvgText(ctx(), 10, 10 + y, "Penny", nullptr);
					break;
				case CoinType::Nickel:
					nvgText(ctx(), 10, 10 + y, "Nickel", nullptr);
					break;
				case CoinType::Dime:
					nvgText(ctx(), 10, 10 + y, "Dime", nullptr);
					break;
				case CoinType::Quarter:
					nvgText(ctx(), 10, 10 + y, "Quarter", nullptr);
					break;
			}
			y += 20;
		}
	}
	void CoinQueue::onEvent(const Cinnabar::EventBroker::Event* event)
	{
		if(event->is<CoinInsertEvent>())
		{
			onEvent(event->as<CoinInsertEvent>());
		}
		else if(event->is<CoinConsumeEvent>())
		{
			onEvent(event->as<CoinConsumeEvent>());
		}
	}
	void CoinQueue::onEvent(const CoinInsertEvent& event)
	{
		_coinQueue.push_back(event.coinType);
	}
	void CoinQueue::onEvent(const CoinConsumeEvent& event)
	{
		_coinQueue.pop_front();
	}
}
