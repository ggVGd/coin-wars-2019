#pragma once
#include "Cinnabar/NanoVGModule.h"
#include "UIWidgets/Canvas.h"

class UI : public Cinnabar::NanoVGModule::Canvas
{
public:
	NVGcontext* ctx() const;
	void render() override;
	UIWidgets::Canvas& rootWidget();
	Cinnabar::Vector2 canvasSize() const;

	bool mouseInside(int x, int y);
	void mousePress(int x, int y);

private:
	UIWidgets::Canvas _rootWidget;
};
