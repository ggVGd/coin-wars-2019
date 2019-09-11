#pragma once
#include "UIWidget.h"
#include "../Cinnabar/EventBroker.h"
#include "../Events.h"

namespace UIWidgets
{
	class TeamPicker : public UIWidget, Cinnabar::EventBroker::Observer
	{
	public:
		void bind(Cinnabar::EventBroker&);

		void onEvent(const Cinnabar::EventBroker::Event* event) override;
		void onEvent(const DepartmentDeselectEvent&);

		void render() override;

		bool mouseInside(int x, int y) override;
		bool mousePress(int x, int y) override;

	private:
		bool _visible = true;
	};
}
