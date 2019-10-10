#pragma once
#include <set>
#include <typeinfo>
#include <unordered_map>

namespace Cinnabar
{
	class EventBroker
	{
	public:
		typedef std::size_t EventType;

		class Observer;
		void addObserver(Observer*);
		void removeObserver(Observer*);
		void subscribe(Observer*, EventType);
		void unsubscribe(Observer*, EventType);

		struct Event;

		template<typename T>
		struct EventContainer;

		void _emit(Event*);

		template<typename T>
		void emit(T t)
		{
			_emit(new EventContainer<T>(t));
		}

	private:
		typedef std::set<Observer*> ObserverSet;
		std::set<Observer*> _observers;
		std::unordered_map<EventType, ObserverSet> _subs;
	};

	class EventBroker::Observer
	{
	public:
		virtual ~Observer();
		virtual void onEvent(const Event*) = 0;
	protected:
		EventBroker* eventBroker() const;
		void subscribe(EventType);
		void unsubscribe(EventType);

		template<typename T>
		void subscribe()
		{
			subscribe(typeid(T).hash_code());
		}

		template<typename T>
		void unsubscribe()
		{
			unsubscribe(typeid(T).hash_code());
		}

	private:
		EventBroker* _broker = nullptr;
		friend class EventBroker;
	};

	struct EventBroker::Event
	{
		Event(const EventType&);
		const EventType type;

		template<typename T>
		bool is() const
		{
			return typeid(T).hash_code() == type;
		}

		template<typename T>
		T as() const
		{
			return static_cast<const EventContainer<T>*>(this)->data;
		}
	};

	template<typename T>
	struct EventBroker::EventContainer : public Event
	{
		EventContainer(T t) : Event(typeid(T).hash_code()), data(t) {}
		const T data;
	};
}
