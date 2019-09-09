#pragma once
#include "Sink.h"
#include <fstream>

namespace Cinnabar {
namespace Logging
{
	class FileSink : public Sink
	{
	public:
		FileSink(const std::string&);
		~FileSink();
		void dispatch(Message) override;

	private:
		std::ofstream _file;
		int _fileWidth = 20;
		int _lineWidth = 5;
	};
}}
