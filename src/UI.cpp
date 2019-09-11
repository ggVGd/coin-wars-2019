#include "UI.h"

NVGcontext* UI::ctx() const
{
	return nvg();
}
void UI::render()
{
	_rootWidget._ui = this;
	_rootWidget._render();
}
UIWidgets::Canvas& UI::rootWidget()
{
	return _rootWidget;
}
Cinnabar::Vector2 UI::canvasSize() const
{
	return size();
}
bool UI::mouseInside(int x, int y)
{
	return _rootWidget._mouseInside(x, y);
}
void UI::mousePress(int x, int y)
{
	_rootWidget._mousePress(x, y);
}
