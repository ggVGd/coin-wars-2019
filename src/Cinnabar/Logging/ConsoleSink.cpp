#include "ConsoleSink.h"
#include <algorithm>
#include <cstring>
#include <iostream>

namespace Cinnabar {
namespace Logging
{
	void ConsoleSink::dispatch(Message message)
	{
		static const char* verbosityStrings[] = {
			"     ",
			"FATAL",
			"ERROR",
			" WARN",
			" INFO",
			"DEBUG"
		};
		static const char* verbosityStyles[] = {
			"",
			"\e[1;31m",
			"\e[1;31m",
			"\e[1;33m"
		};

		constexpr auto verbosityStringsCount = sizeof(verbosityStrings) / sizeof(verbosityStrings[0]);
		constexpr auto verbosityStylesCount = sizeof(verbosityStyles) / sizeof(verbosityStyles[0]);

		static const char* format = "%s%*s:%-*d %s | %s\e(B\e[m\r\n";

		const int fileWidth = strlen(message.filename);
		const int lineWidth = snprintf(nullptr, 0, "%d", message.line);
		_fileWidth = std::max(_fileWidth, fileWidth + 1);
		_lineWidth = std::max(_lineWidth, lineWidth + 1);

		int len = snprintf(nullptr, 0, format,
			message.verbosity < verbosityStylesCount ? verbosityStyles[message.verbosity] : "",
			_fileWidth,
			message.filename,
			_lineWidth,
			message.line,
			message.verbosity < verbosityStringsCount ? verbosityStrings[message.verbosity] : "",
			message.message);
		char* buf = new char[len+1];
		snprintf(buf, len+1, format,
			message.verbosity < verbosityStylesCount ? verbosityStyles[message.verbosity] : "",
			_fileWidth,
			message.filename,
			_lineWidth,
			message.line,
			message.verbosity < verbosityStringsCount ? verbosityStrings[message.verbosity] : "",
			message.message);

		std::cout << buf;

		delete[] buf;
	}
}}
