#include "EventBroker.h"

namespace Cinnabar
{
	void EventBroker::addObserver(Observer* observer)
	{
		observer->_broker = this;
		_observers.insert(observer);
	}
	void EventBroker::removeObserver(Observer* observer)
	{
		_observers.erase(observer);
		for(auto& pair : _subs)
		{
			pair.second.erase(observer);
		}
	}
	void EventBroker::subscribe(Observer* observer, EventType type)
	{
		addObserver(observer);
		_subs[type].insert(observer);
	}
	void EventBroker::unsubscribe(Observer* observer, EventType type)
	{
		_subs[type].erase(observer);
	}
	void EventBroker::_emit(Event* event)
	{
		for(Observer* observer : _subs[event->type])
		{
			observer->onEvent(event);
		}
	}

	EventBroker::Observer::~Observer()
	{
		if(_broker)
		{
			_broker->removeObserver(this);
		}
	}
	void EventBroker::Observer::subscribe(EventType type)
	{
		_broker->subscribe(this, type);
	}
	void EventBroker::Observer::unsubscribe(EventType type)
	{
		_broker->unsubscribe(this, type);
	}

	EventBroker::Event::Event(const EventType& type_) : type(type_)
	{
	}
}
