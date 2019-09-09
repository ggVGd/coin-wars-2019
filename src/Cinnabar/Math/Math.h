#pragma once

namespace Cinnabar
{
	template<typename T>
	T min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template<typename T>
	T max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	template<typename T>
	T clamp(T x, T a, T b)
	{
		return a < b ? min(max(x, a), b) : max(min(x, a), b);
	}
}
