#include "HUD.h"
#include "../Database.h"
#include "../Settings.h"

namespace UIWidgets
{
	void HUD::bind(Cinnabar::EventBroker& eventBroker)
	{
		eventBroker.addObserver(this);
		subscribe<CoinInsertEvent>();
		subscribe<CoinConsumeEvent>();
		subscribe<DepartmentSelectEvent>();
		subscribe<DepartmentDeselectEvent>();
		subscribe<GivePointsEvent>();
	}
	void HUD::update(float elapsed)
	{
		const auto flyingTextSpeed = Cinnabar::Vector2(0, -30.0f) * elapsed;

		for(auto it = _flyingTexts.begin(); it != _flyingTexts.end();)
		{
			it->timer += elapsed;
			it->position += flyingTextSpeed;
			if(it->timer >= it->maxTimer)
				it = _flyingTexts.erase(it);
			else
				it++;
		}
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
			const auto coinText = Settings::coinText(coin);
			const auto coinValue = Settings::coinValue(coin);
			const auto text = coinText + " (x" + std::to_string(coinValue) + ")";
			nvgText(ctx(), 10, 40 + y, text.c_str(), nullptr);
			y += 20;
		}

		nvgTextAlign(ctx(), NVG_ALIGN_CENTER | NVG_ALIGN_BOTTOM);
		const auto& buckets = Settings::buckets();
		const float bucket_height = Settings::get<float>("bucket_height");
		const auto bucket_width = canvasSize().x / Settings::bucketSizeFactors();
		float x0 = 0, x1 = 0;
		for(std::size_t i = 0; i < buckets.size(); i++)
		{
			const auto& bucket = buckets.at(i);
			x1 = x0 + bucket_width * bucket.sizeFactor;
			const float x = (x0 + x1) * 0.5f;
			x0 = x1;

			if(!bucket.label.empty())
			{
				nvgFontSize(ctx(), 40.0f);
				nvgText(ctx(), x, canvasSize().y - bucket_height + 30.0f, bucket.label.c_str(), nullptr);
			}

			std::string text = std::to_string(bucket.multiplier);
			nvgFontSize(ctx(), 50.0f);
			nvgText(ctx(), x, canvasSize().y - bucket_height + 90.0f, text.c_str(), nullptr);
		}

		nvgFontFaceId(ctx(), nvgFindFont(ctx(), "OpenSans-Bold"));
		nvgTextAlign(ctx(), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
		nvgFontSize(ctx(), 40.0f);
		for(const auto& flyingText : _flyingTexts)
		{
			const float scale = 1.0f - flyingText.timer / flyingText.maxTimer;
			nvgFillColor(ctx(), nvgRGBA(255, 255, 255, scale * 255));
			nvgText(ctx(), flyingText.position.x, flyingText.position.y, flyingText.text.c_str(), nullptr);
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
		else if(event->is<GivePointsEvent>())
		{
			onEvent(event->as<GivePointsEvent>());
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
	void HUD::onEvent(const GivePointsEvent& event)
	{
		if(event.points <= 0)
			return;

		const auto text = "+" + std::to_string(event.points);
		const Cinnabar::Vector2 position(
			event.position.x,
			canvasSize().y - event.position.y
		);

		_flyingTexts.push_back({
			0.0f,
			2.0f,
			position,
			text
		});
	}
}
