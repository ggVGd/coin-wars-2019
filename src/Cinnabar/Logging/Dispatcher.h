#pragma once
#include <memory>
#include <set>

namespace Cinnabar {
namespace Logging
{
	class Message;
	class Sink;

	class Dispatcher
	{
		Dispatcher();
	public:
		static Dispatcher& getSingleton();

		const std::set<std::shared_ptr<Sink>>& sinks() const;
		void addSink(std::shared_ptr<Sink>);
		void removeSink(std::shared_ptr<Sink>);

		void dispatch(Message);

	private:
		std::set<std::shared_ptr<Sink>> _sinks;
	};
}}
