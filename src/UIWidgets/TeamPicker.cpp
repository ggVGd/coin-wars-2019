#include "TeamPicker.h"
#include "../Cinnabar/Core.h"
#include "../Cinnabar/ThirdParty/nanovg/nanovg.h"
#include "../Database.h"

#define GEOMETRY \
	const Cinnabar::Vector2 dialogSize(500, 500); \
	const Cinnabar::Vector2 dialogPosition = canvasSize() * 0.5f - dialogSize * 0.5f; \
	const float rowHeight = 40.0f; \
	const float rowSpacing = 10.0f; \
	const float padding = 20.0f;

namespace UIWidgets
{
	void TeamPicker::bind(Cinnabar::EventBroker& eventBroker)
	{
		eventBroker.addObserver(this);
		subscribe<DepartmentDeselectEvent>();
	}
	void TeamPicker::onEvent(const Cinnabar::EventBroker::Event* event)
	{
		if(event->is<DepartmentDeselectEvent>())
		{
			onEvent(event->as<DepartmentDeselectEvent>());
		}
	}
	void TeamPicker::onEvent(const DepartmentDeselectEvent& event)
	{
		_visible = true;
	}
	void TeamPicker::render()
	{
		if(!_visible)
			return;

		GEOMETRY

		nvgBeginPath(ctx());
		nvgRect(ctx(), dialogPosition.x, dialogPosition.y, dialogSize.x, dialogSize.y);
		nvgFillColor(ctx(), nvgRGBA(0, 0, 0, 200));
		nvgFill(ctx());
		nvgStrokeColor(ctx(), nvgRGBA(255, 255, 255, 255));
		nvgStroke(ctx());

		nvgFontSize(ctx(), 40.0f);
		nvgFontFaceId(ctx(), nvgFindFont(ctx(), "OpenSans-Bold"));
		nvgTextAlign(ctx(), NVG_ALIGN_CENTER);
		nvgFontBlur(ctx(), 0);
		nvgFillColor(ctx(), nvgRGBA(255, 255, 255, 255));
		nvgText(ctx(), canvasSize().x * 0.5f, dialogPosition.y + 60.0f, "Pick Your Department", nullptr);

		nvgFontSize(ctx(), 30.0f);
		nvgFontFaceId(ctx(), nvgFindFont(ctx(), "OpenSans-Regular"));
		nvgFontBlur(ctx(), 0);

		auto& db = Database::getSingleton();
		int row = 0;
		for(auto item : db.getPoints())
		{
			const auto points = std::to_string(item.points);

			const float rowY = dialogPosition.y + 100.0f + row * (rowHeight + rowSpacing);

			nvgBeginPath(ctx());
			nvgRect(ctx(), dialogPosition.x + padding, rowY, dialogSize.x - padding * 2.0f, rowHeight);
			nvgFillColor(ctx(), nvgRGBA(255, 255, 255, 100));
			nvgFill(ctx());

			nvgFillColor(ctx(), nvgRGBA(255, 255, 255, 255));
			nvgTextAlign(ctx(), NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
			nvgText(ctx(), dialogPosition.x + padding * 2.0f, rowY + rowHeight * 0.5f, item.department.c_str(), nullptr);

			nvgTextAlign(ctx(), NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
			nvgText(ctx(), dialogPosition.x + dialogSize.x - padding * 2.0f, rowY + rowHeight * 0.5f, points.c_str(), nullptr);

			row++;
		}
	}
	bool TeamPicker::mouseInside(int x, int y)
	{
		if(!_visible)
			return false;

		GEOMETRY

		return x >= dialogPosition.x
			&& x <= dialogPosition.x + dialogSize.x
			&& y >= dialogPosition.y
			&& y <= dialogPosition.y + dialogSize.y;
	}
	bool TeamPicker::mousePress(int x, int y)
	{
		if(!_visible)
			return false;

		GEOMETRY

		auto& db = Database::getSingleton();
		int row = 0;
		for(auto item : db.getPoints())
		{
			const float rowY = dialogPosition.y + 100.0f + row * (rowHeight + rowSpacing);
			if(y >= rowY && y <= rowY + rowHeight)
			{
				_visible = false;
				Cinnabar::Core::getSingleton()->eventBroker().emit<DepartmentSelectEvent>({ item.department });
				return true;
			}
			row++;
		}
		return false;
	}
}
