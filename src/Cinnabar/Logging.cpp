#include "Logging.h"
#include "Logging/Dispatcher.h"
#include <cstdarg>
#include <vector>

namespace Cinnabar {
namespace Logging
{
	void log(Verbosity verbosity, const char* filename, int line, const char* str)
	{
		Message message;
		message.verbosity = verbosity;
		message.filename = filename;
		message.line = line;
		message.message = str;
		dispatch(message);
	}

	void logf(Verbosity verbosity, const char* filename, int line, const char* format, ...)
	{
		va_list vl;
		va_start(vl, format);
		int len = vsnprintf(nullptr, 0, format, vl);
		va_end(vl);

		va_start(vl, format);
		char* buf = new char[len+1];
		vsnprintf(buf, len + 1, format, vl);
		va_end(vl);

		log(verbosity, filename, line, buf);

		delete[] buf;
	}

	Stream::Stream(Verbosity verbosity, const char* filename, int line)
		: _verbosity(verbosity), _filename(filename), _line(line)
	{
	}
	Stream::Stream(Stream&& other) : _ss(std::move(other._ss))
	{
	}
	Stream::~Stream()
	{
		log(_verbosity, _filename, _line, _ss.str().c_str());
	}
	Stream logs(Verbosity verbosity, const char* filename, int line)
	{
		return Stream(verbosity, filename, line);
	}
}}
