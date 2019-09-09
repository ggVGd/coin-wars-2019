#pragma once
#include "../Logging.h"

namespace Cinnabar {
namespace Logging
{
	class Sink
	{
	public:
		virtual ~Sink() = default;
		virtual void dispatch(Message) = 0;
	};
}}
