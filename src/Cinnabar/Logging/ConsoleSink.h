#pragma once
#include "Sink.h"

namespace Cinnabar {
namespace Logging
{
	class ConsoleSink : public Sink
	{
	public:
		void dispatch(Message) override;

	private:
		int _fileWidth = 20;
		int _lineWidth = 5;
	};
}}
