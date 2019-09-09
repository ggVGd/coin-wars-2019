#include "Dispatcher.h"
#include "ConsoleSink.h"
#include "Sink.h"
#include "../Logging.h"

namespace Cinnabar {
namespace Logging
{
	Dispatcher::Dispatcher()
	{
		_sinks.insert(std::make_shared<ConsoleSink>());
	}
	Dispatcher& Dispatcher::getSingleton()
	{
		static Dispatcher instance;
		return instance;
	}
	const std::set<std::shared_ptr<Sink>>& Dispatcher::sinks() const
	{
		return _sinks;
	}
	void Dispatcher::addSink(std::shared_ptr<Sink> sink)
	{
		_sinks.insert(sink);
	}
	void Dispatcher::removeSink(std::shared_ptr<Sink> sink)
	{
		_sinks.erase(sink);
	}

	void Dispatcher::dispatch(Message message)
	{
		for(const auto& sink : _sinks)
		{
			sink->dispatch(message);
		}
	}

	void addSink(std::shared_ptr<Sink> sink)
	{
		Dispatcher::getSingleton().addSink(sink);
	}
	void dispatch(Message message)
	{
		Dispatcher::getSingleton().dispatch(message);
	}
}}
