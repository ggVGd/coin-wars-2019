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
		nvgFontFaceId(ctx(), nvgFindFont(ctx(), "OpenSans-Bold"));
		nvgTextAlign(ctx(), NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
		nvgFontBlur(ctx(), 0);
		nvgFillColor(ctx(), nvgRGBA(255, 255, 255, 255));

		if(!_department.empty())
		{
			auto& db = Database::getSingleton();
			std::string text = _department + ": " + std::to_string(db.getPoints(_department)) + " points";
			nvgFontSize(ctx(), 40.0f);
			nvgText(ctx(), canvasSize().x * 0.5f, 10.0f, text.c_str(), nullptr);
			nvgFontSize(ctx(), 30.0f);
			nvgText(ctx(), canvasSize().x * 0.5f, 40.0f, "(press here to change)", nullptr);
		}

		nvgFontFaceId(ctx(), nvgFindFont(ctx(), "OpenSans-Regular"));
		nvgFontSize(ctx(), 30.0f);
		nvgTextAlign(ctx(), NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
		nvgText(ctx(), 10.0f, 10.0f, "Coin Queue:", nullptr);

		nvgFontSize(ctx(), 20.0f);
		int y = 0;
		for(const auto& coin : _coinQueue)
		{
			switch(coin)
			{
				case CoinType::Penny:
					nvgText(ctx(), 10, 40 + y, "Penny", nullptr);
					break;
				case CoinType::Nickel:
					nvgText(ctx(), 10, 40 + y, "Nickel", nullptr);
					break;
				case CoinType::Dime:
					nvgText(ctx(), 10, 40 + y, "Dime", nullptr);
					break;
				case CoinType::Quarter:
					nvgText(ctx(), 10, 40 + y, "Quarter", nullptr);
					break;
			}
			y += 20;
		}
	}
	bool HUD::mouseInside(int x, int y)
	{
		if(fabs(canvasSize().x * 0.5f - x) < 200.0f && y < 80.0f)
			return true;
		return false;
	}
	bool HUD::mousePress(int x, int y)
	{
		if(fabs(canvasSize().x * 0.5f - x) < 200.0f && y < 80.0f)
		{
			eventBroker()->emit(DepartmentDeselectEvent());
			return true;
		}
		return false;
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
