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
		bool mouseInside(int x, int y) override;
		bool mousePress(int x, int y) override;

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
