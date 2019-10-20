#include "TeamPicker.h"
#include "../Cinnabar/Core.h"
#include "../Cinnabar/ThirdParty/nanovg/nanovg.h"
#include "../Database.h"
#include "../Settings.h"

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

		auto g = _calculateGeometry();

		nvgBeginPath(ctx());
		nvgRect(ctx(), g.dialogPosition.x, g.dialogPosition.y, g.dialogSize.x, g.dialogSize.y);
		nvgFillColor(ctx(), nvgRGBA(0, 0, 0, 200));
		nvgFill(ctx());
		nvgStrokeColor(ctx(), nvgRGBA(255, 255, 255, 255));
		nvgStroke(ctx());

		nvgFontSize(ctx(), 40.0f);
		nvgFontFaceId(ctx(), nvgFindFont(ctx(), "OpenSans-Bold"));
		nvgTextAlign(ctx(), NVG_ALIGN_TOP | NVG_ALIGN_CENTER);
		nvgFontBlur(ctx(), 0);
		nvgFillColor(ctx(), nvgRGBA(255, 255, 255, 255));
		nvgText(ctx(), canvasSize().x * 0.5f, g.dialogPosition.y + 10.0f, "Pick Your Department", nullptr);

		nvgFontSize(ctx(), g.rowHeight - g.rowPadding * 2.0f);
		nvgFontFaceId(ctx(), nvgFindFont(ctx(), "OpenSans-Regular"));
		nvgFontBlur(ctx(), 0);

		auto& db = Database::getSingleton();
		int row = 0;
		for(auto item : db.getPoints())
		{
			const auto points = std::to_string(item.points);

			const float rowY = g.dialogPosition.y + 60.0f + row * (g.rowHeight + g.rowSpacing);

			nvgBeginPath(ctx());
			nvgRect(ctx(), g.dialogPosition.x + g.padding, rowY, g.dialogSize.x - g.padding * 2.0f, g.rowHeight);
			nvgFillColor(ctx(), nvgRGBA(255, 255, 255, 100));
			nvgFill(ctx());

			nvgFillColor(ctx(), nvgRGBA(255, 255, 255, 255));
			nvgTextAlign(ctx(), NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
			nvgText(ctx(), g.dialogPosition.x + g.padding * 2.0f, rowY + g.rowHeight * 0.5f, item.department.c_str(), nullptr);

			nvgTextAlign(ctx(), NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
			nvgText(ctx(), g.dialogPosition.x + g.dialogSize.x - g.padding * 2.0f, rowY + g.rowHeight * 0.5f, points.c_str(), nullptr);

			row++;
		}
	}
	bool TeamPicker::mouseInside(int x, int y)
	{
		if(!_visible)
			return false;

		auto g = _calculateGeometry();

		return x >= g.dialogPosition.x
			&& x <= g.dialogPosition.x + g.dialogSize.x
			&& y >= g.dialogPosition.y
			&& y <= g.dialogPosition.y + g.dialogSize.y;
	}
	bool TeamPicker::mousePress(int x, int y)
	{
		if(!_visible)
			return false;

		auto g = _calculateGeometry();

		auto& db = Database::getSingleton();
		int row = 0;
		for(auto item : db.getPoints())
		{
			const float rowY = g.dialogPosition.y + 60.0f + row * (g.rowHeight + g.rowSpacing);
			if(y >= rowY && y <= rowY + g.rowHeight)
			{
				_visible = false;
				Cinnabar::Core::getSingleton()->eventBroker().emit<DepartmentSelectEvent>({ item.department });
				return true;
			}
			row++;
		}
		return false;
	}
	TeamPicker::Geometry TeamPicker::_calculateGeometry()
	{
		const int rows = Database::getSingleton().getPoints().size();

		Geometry geometry;

		geometry.rowHeight = Settings::get<float>("teamPicker_rowHeight");
		geometry.rowSpacing = Settings::get<float>("teamPicker_rowSpacing");
		geometry.rowPadding = Settings::get<float>("teamPicker_rowPadding");
		geometry.padding = Settings::get<float>("teamPicker_padding");

		geometry.dialogSize = { 500, 80 + rows * (geometry.rowHeight + geometry.rowSpacing) };
		geometry.dialogPosition = canvasSize() * 0.5f - geometry.dialogSize * 0.5f;

		return geometry;
	}
}
