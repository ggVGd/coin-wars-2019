#pragma once
#include "UIWidget.h"
#include "../Cinnabar/EventBroker.h"
#include "../Events.h"
#include <deque>
#include <vector>

namespace UIWidgets
{
	class HUD : public UIWidget, Cinnabar::EventBroker::Observer
	{
	public:
		void bind(Cinnabar::EventBroker&);
		void update(float elapsed) override;
		void render() override;
		bool mouseInside(int x, int y) override;
		bool mousePress(int x, int y) override;

		void onEvent(const Cinnabar::EventBroker::Event*) override;
		void onEvent(const CoinInsertEvent&);
		void onEvent(const CoinConsumeEvent&);
		void onEvent(const DepartmentSelectEvent&);
		void onEvent(const DepartmentDeselectEvent&);
		void onEvent(const GivePointsEvent&);

	private:
		std::deque<CoinType> _coinQueue;
		std::string _department;

		struct FlyingText
		{
			float timer;
			float maxTimer;
			Cinnabar::Vector2 position;
			std::string text;
		};
		std::vector<FlyingText> _flyingTexts;
	};
}
