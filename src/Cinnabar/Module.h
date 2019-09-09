#pragma once
#include <SDL2/SDL.h>
#include <typeinfo>

namespace Cinnabar
{
	class Core;

	class ModuleBase
	{
	public:
		virtual ~ModuleBase() = default;
		virtual std::size_t moduleType() const = 0;
		virtual void init() = 0;
		virtual void shutdown() = 0;
		virtual void update(float) = 0;
	
	protected:
		Core* core() const;

	private:
		friend class Core;
		Core* _core;
	};

	template<typename T>
	class Module : public ModuleBase
	{
	public:
		virtual ~Module() = default;
		virtual inline std::size_t moduleType() const final
		{
			return typeid(T).hash_code();
		}
	};
}
