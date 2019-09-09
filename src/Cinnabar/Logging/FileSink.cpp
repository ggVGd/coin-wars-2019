#include "FileSink.h"
#include <algorithm>
#include <cstring>
#include <iostream>

namespace Cinnabar {
namespace Logging
{
	FileSink::FileSink(const std::string& filename)
		: _file(filename)
	{
	}
	FileSink::~FileSink()
	{
		if(_file.is_open())
			_file.close();
	}
	void FileSink::dispatch(Message message)
	{
		if(!_file.is_open())
			return;

		static const char* verbosityStrings[] = {
			"     ",
			"FATAL",
			"ERROR",
			" WARN",
			" INFO",
			"DEBUG"
		};

		constexpr auto verbosityStringsCount = sizeof(verbosityStrings) / sizeof(verbosityStrings[0]);

		static const char* format = "%*s:%-*d %s | %s\r\n";

		const int fileWidth = strlen(message.filename);
		const int lineWidth = snprintf(nullptr, 0, "%d", message.line);
		_fileWidth = std::max(_fileWidth, fileWidth + 1);
		_lineWidth = std::max(_lineWidth, lineWidth + 1);

		int len = snprintf(nullptr, 0, format,
			_fileWidth,
			message.filename,
			_lineWidth,
			message.line,
			message.verbosity < verbosityStringsCount ? verbosityStrings[message.verbosity] : "",
			message.message);
		char* buf = new char[len+1];
		snprintf(buf, len+1, format,
			_fileWidth,
			message.filename,
			_lineWidth,
			message.line,
			message.verbosity < verbosityStringsCount ? verbosityStrings[message.verbosity] : "",
			message.message);

		_file.write(buf, len);
		_file.flush();

		delete[] buf;
	}
}}
