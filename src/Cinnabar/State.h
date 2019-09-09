#pragma once

namespace Cinnabar
{
	class Core;

	class State
	{
	public:
		virtual void enter() = 0;
		virtual void leave() = 0;
		virtual void render() = 0;
		virtual void update(float) = 0;

	protected:
		Core* core() const;
	
	private:
		friend class Core;
		Core* _core;
	};
}
