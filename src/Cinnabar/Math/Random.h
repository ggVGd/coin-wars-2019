#pragma once

namespace Cinnabar {
namespace Math
{
	// Generate a random number in [0, 1].
	float randUnit();

	// Generate a random number in [-1, 1].
	float randSymmetric();

	// Generate a random T in [min, max].
	template<typename T>
	T randRange(const T& min, const T& max)
	{
		return min + (max - min) * randUnit();
	}
}}
