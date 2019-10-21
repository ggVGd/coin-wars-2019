#pragma once
#include "../Cinnabar/Math/Vector2.h"
#include "../Cinnabar/ThirdParty/nanovg/nanovg.h"
#include <memory>
#include <set>

class UI;

namespace UIWidgets
{
	class UIWidget
	{
	public:
		void _update(float elapsed);
		inline virtual void update(float){}

		void _render();
		virtual void render() = 0;

		Cinnabar::Vector2 canvasSize() const;

		void addChild(std::shared_ptr<UIWidget>);
		void removeChild(std::shared_ptr<UIWidget>);

		template<typename T, typename... Args>
		std::shared_ptr<T> addChild(Args... args)
		{
			auto child = std::make_shared<T>(args...);
			addChild(child);
			return child;
		}

		bool _mouseInside(int x, int y);
		virtual bool mouseInside(int x, int y);

		bool _mousePress(int x, int y);
		virtual bool mousePress(int x, int y);

	protected:
		NVGcontext* ctx() const;

	private:
		UI* _ui;
		UIWidget* _parent = nullptr;
		std::set<std::shared_ptr<UIWidget>> _children;

		friend class ::UI;
	};
}
