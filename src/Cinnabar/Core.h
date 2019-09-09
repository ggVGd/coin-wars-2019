#pragma once
#include "EventBroker.h"
#include <memory>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>

namespace Cinnabar
{
	class ModuleBase;
	class State;

	class Core
	{
	public:
		void init();
		void shutdown();

		void run();
		void stop();

		EventBroker& eventBroker();

		void addModule(std::shared_ptr<ModuleBase> module);
		std::shared_ptr<ModuleBase> module(std::size_t);
		
		template<typename T>
		std::weak_ptr<T> addModule()
		{
			auto module = std::make_shared<T>();
			addModule(module);
			return module;
		}

		template<typename T>
		std::shared_ptr<T> module()
		{
			const auto type = typeid(T).hash_code();
			return std::dynamic_pointer_cast<T>(module(type));
		}

		void pushState(std::shared_ptr<State> state);
		void popState();
		std::shared_ptr<State> state() const;

		template<typename T>
		std::shared_ptr<T> pushState()
		{
			auto state = std::make_shared<T>();
			pushState(state);
			return state;
		}

	private:
		EventBroker _eventBroker;
		bool _running;
		std::vector<std::shared_ptr<ModuleBase>> _modules;
		std::unordered_map<std::size_t, std::shared_ptr<ModuleBase>> _moduleMap;
		std::stack<std::shared_ptr<State>> _states;
		std::queue<std::shared_ptr<State>> _stateChanges;
	};
}
