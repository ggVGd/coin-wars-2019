#pragma once
#include <functional>
#include <vector>

namespace Cinnabar
{
	template<typename... T>
	class Signal
	{
	public:
		typedef std::function<void(T...)> Slot;

		void operator()(T...);

		void subscribe(void*, Slot);	
		void subscribe(Slot);
		void unsubscribe(void*);

	private:
		struct Sub
		{
			void* owner;
			Slot slot;
		};
		std::vector<Sub> _subs;
	};
		
	template<typename... T>
	void Signal<T...>::operator()(T... args)
	{
		for(auto& sub : _subs)
			sub.slot(args...);
	}

	template<typename... T>
	void Signal<T...>::subscribe(void* owner, typename Signal<T...>::Slot slot)
	{
		Sub sub;
		sub.owner = owner;
		sub.slot = slot;
		_subs.push_back(sub);
	}

	template<typename... T>
	void Signal<T...>::subscribe(typename Signal<T...>::Slot slot)
	{
		subscribe(nullptr, slot);
	}

	template<typename... T>
	void Signal<T...>::unsubscribe(void* owner)
	{
		for(auto it = _subs.begin(); it != _subs.end();)
		{
			if(it->owner == owner)
				it = _subs.erase(it);
			else
				it++;
		}
	}
}
