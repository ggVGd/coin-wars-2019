#pragma once
#include "UIWidget.h"
#include "../Cinnabar/EventBroker.h"
#include "../Events.h"
#include <deque>

namespace UIWidgets
{
	class CoinQueue : public UIWidget, Cinnabar::EventBroker::Observer
	{
	public:
		void bind(Cinnabar::EventBroker&);
		void render() override;

		void onEvent(const Cinnabar::EventBroker::Event*) override;
		void onEvent(const CoinInsertEvent&);
		void onEvent(const CoinConsumeEvent&);

	private:
		std::deque<CoinType> _coinQueue;
	};
}
