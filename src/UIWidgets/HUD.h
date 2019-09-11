#pragma once
#include "UIWidget.h"
#include "../Cinnabar/EventBroker.h"
#include "../Events.h"
#include <deque>

namespace UIWidgets
{
	class HUD : public UIWidget, Cinnabar::EventBroker::Observer
	{
	public:
		void bind(Cinnabar::EventBroker&);
		void render() override;

		void onEvent(const Cinnabar::EventBroker::Event*) override;
		void onEvent(const CoinInsertEvent&);
		void onEvent(const CoinConsumeEvent&);
		void onEvent(const DepartmentSelectEvent&);
		void onEvent(const DepartmentDeselectEvent&);

	private:
		std::deque<CoinType> _coinQueue;
		std::string _department;
	};
}
