#include "Core.h"
#include "Module.h"
#include "State.h"
#include <chrono>

namespace Cinnabar
{
	Core* Core::_instance = nullptr;

	Core::Core()
	{
		if(_instance)
			throw std::runtime_error("There can only be one Cinnabar::Core.");
		_instance = this;
	}
	Core::~Core()
	{
		_instance = nullptr;
	}
	Core* Core::getSingleton()
	{
		return _instance;
	}
	void Core::init()
	{
		for(auto& module : _modules)
		{
			module->init();
		}
	}
	void Core::shutdown()
	{
		for(auto it = _modules.rbegin(); it != _modules.rend(); it++)
		{
			(*it)->shutdown();
		}
	}
	void Core::run()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
		start = std::chrono::high_resolution_clock::now();
		float elapsed;

		_running = true;
		while(_running)
		{
			end = std::chrono::high_resolution_clock::now();
			elapsed = std::chrono::duration<float>(end - start).count();
			start = end;

			for(auto& module : _modules)
			{
				module->update(elapsed);
			}

			if(!_states.empty())
			{
				_states.top()->update(elapsed);
				_states.top()->render();
			}
			
			while(!_stateChanges.empty())
			{
				auto change = _stateChanges.front();
				_stateChanges.pop();

				if(change)
				{
					_states.push(change);
					change->enter();
				}
				else
				{
					_states.top()->leave();
					_states.pop();
				}
			}
		}
	}
	void Core::stop()
	{
		_running = false;
	}
	EventBroker& Core::eventBroker()
	{
		return _eventBroker;
	}
	void Core::addModule(std::shared_ptr<ModuleBase> module)
	{
		module->_core = this;
		_modules.push_back(module);
		_moduleMap[module->moduleType()] = module;
	}
	std::shared_ptr<ModuleBase> Core::module(std::size_t type)
	{
		return _moduleMap[type];
	}
	void Core::pushState(std::shared_ptr<State> state)
	{
		state->_core = this;
		_stateChanges.push(state);
	}
	void Core::popState()
	{
		_stateChanges.push(nullptr);
	}
	std::shared_ptr<State> Core::state() const
	{
		if(_states.empty())
			return nullptr;
		return _states.top();
	}
}
