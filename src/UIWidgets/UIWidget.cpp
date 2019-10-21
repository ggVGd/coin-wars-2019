#include "UIWidget.h"
#include "../UI.h"

namespace UIWidgets
{
	void UIWidget::_update(float elapsed)
	{
		update(elapsed);
		for(auto child : _children)
		{
			child->_ui = _ui;
			child->_parent = this;
			child->_update(elapsed);
		}
	}
	void UIWidget::_render()
	{
		render();
		for(auto child : _children)
		{
			child->_ui = _ui;
			child->_parent = this;
			child->_render();
		}
	}
	Cinnabar::Vector2 UIWidget::canvasSize() const
	{
		return _ui->canvasSize();
	}
	void UIWidget::addChild(std::shared_ptr<UIWidget> child)
	{
		_children.insert(child);
	}
	void UIWidget::removeChild(std::shared_ptr<UIWidget> child)
	{
		_children.erase(child);
	}
	bool UIWidget::_mouseInside(int x, int y)
	{
		if(mouseInside(x, y))
			return true;
		for(auto child : _children)
		{
			if(child->_mouseInside(x, y))
				return true;
		}
		return false;
	}
	bool UIWidget::mouseInside(int x, int y)
	{
		return false;
	}
	bool UIWidget::_mousePress(int x, int y)
	{
		for(auto child : _children)
		{
			if(child->_mousePress(x, y))
				return true;
		}
		if(mouseInside(x, y) && mousePress(x, y))
			return true;
		return false;
	}
	bool UIWidget::mousePress(int x, int y)
	{
		return false;
	}
	NVGcontext* UIWidget::ctx() const
	{
		return _ui->ctx();
	}
}
