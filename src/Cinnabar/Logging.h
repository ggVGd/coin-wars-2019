#pragma once
#include <cstdint>
#include <string>
#include <sstream>
#include <functional>
#include <memory>

namespace Cinnabar {
namespace Logging
{
	enum Verbosity : uint8_t
	{
		Verbosity_OFF,
		Verbosity_FATAL,
		Verbosity_ERROR,
		Verbosity_WARN,
		Verbosity_INFO,
		Verbosity_DEBUG
	};

	struct Message
	{
		Verbosity verbosity;
		const char* filename;
		unsigned int line;
		const char* message;
	};

	void log(Verbosity verbosity, const char* filename, int line, const char* message);
	void logf(Verbosity verbosity, const char* filename, int line, const char* format, ...);

	class Stream
	{
	public:
		Stream(Verbosity verbosity, const char* filename, int line);
		Stream(const Stream&) = delete;
		Stream(Stream&&);
		~Stream();

		template<typename T>
		Stream& operator<<(T t)
		{
			_ss << t;
			return *this;
		}

	private:
		Verbosity _verbosity;
		const char* _filename;
		int _line;
		std::stringstream _ss;
	};
	Stream logs(Verbosity verbosity, const char* filename, int line);

	class Sink;
	void addSink(std::shared_ptr<Sink>);
	void dispatch(Message);

	template<typename T, typename... Args>
	std::shared_ptr<T> addSink(Args... args)
	{
		auto sink = std::make_shared<T>(args...);
		addSink(sink);
		return sink;
	}

	constexpr const char* basename(const char* str, const char* last)
	{
		if(*str == 0)
			return last;
		if(*str == '/')
			return basename(str+1, str+1);
		return basename(str+1, last);
	}
	constexpr const char* basename(const char* str)
	{
		return basename(str, str);
	}
}}

#define LOG(VERBOSITY, MESSAGE) \
	Cinnabar::Logging::log(Cinnabar::Logging::Verbosity_##VERBOSITY, \
		Cinnabar::Logging::basename(__FILE__), __LINE__, MESSAGE)

#define LOGF(VERBOSITY, MESSAGE, ...) \
	Cinnabar::Logging::logf(Cinnabar::Logging::Verbosity_##VERBOSITY, \
		Cinnabar::Logging::basename(__FILE__), __LINE__, MESSAGE, __VA_ARGS__)

#define LOGS(VERBOSITY) \
	Cinnabar::Logging::logs(Cinnabar::Logging::Verbosity_##VERBOSITY, \
		Cinnabar::Logging::basename(__FILE__), __LINE__)

