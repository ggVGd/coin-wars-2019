#include "HUD.h"
#include "../Database.h"

namespace UIWidgets
{
	void HUD::bind(Cinnabar::EventBroker& eventBroker)
	{
		eventBroker.addObserver(this);
		subscribe<CoinInsertEvent>();
		subscribe<CoinConsumeEvent>();
		subscribe<DepartmentSelectEvent>();
		subscribe<DepartmentDeselectEvent>();
	}
	void HUD::render()
	{
		nvgFontSize(ctx(), 40.0f);
		nvgFontFaceId(ctx(), nvgFindFont(ctx(), "OpenSans-Bold"));
		nvgTextAlign(ctx(), NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
		nvgFontBlur(ctx(), 0);
		nvgFillColor(ctx(), nvgRGBA(255, 255, 255, 255));

		if(!_department.empty())
		{
			auto& db = Database::getSingleton();
			std::string text = _department + ": " + std::to_string(db.getPoints(_department)) + " points";
			nvgText(ctx(), canvasSize().x * 0.5f, 10.0f, text.c_str(), nullptr);
		}

		nvgFontFaceId(ctx(), nvgFindFont(ctx(), "OpenSans-Regular"));
		nvgFontSize(ctx(), 20.0f);
		nvgTextAlign(ctx(), NVG_ALIGN_LEFT);

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
	void HUD::onEvent(const Cinnabar::EventBroker::Event* event)
	{
		if(event->is<CoinInsertEvent>())
		{
			onEvent(event->as<CoinInsertEvent>());
		}
		else if(event->is<CoinConsumeEvent>())
		{
			onEvent(event->as<CoinConsumeEvent>());
		}
		else if(event->is<DepartmentSelectEvent>())
		{
			onEvent(event->as<DepartmentSelectEvent>());
		}
		else if(event->is<DepartmentDeselectEvent>())
		{
			onEvent(event->as<DepartmentDeselectEvent>());
		}
	}
	void HUD::onEvent(const CoinInsertEvent& event)
	{
		_coinQueue.push_back(event.coinType);
	}
	void HUD::onEvent(const CoinConsumeEvent& event)
	{
		_coinQueue.pop_front();
	}
	void HUD::onEvent(const DepartmentSelectEvent& event)
	{
		_department = event.department;
	}
	void HUD::onEvent(const DepartmentDeselectEvent& event)
	{
		_department.clear();
	}
}
