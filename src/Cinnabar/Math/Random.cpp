#include "Random.h"
#include <cstdlib>

namespace Cinnabar {
namespace Math
{
	// Divisions are bad!
	static const float RAND_SCALE = 1.0f / RAND_MAX;

	float randUnit()
	{
		return float(rand()) * RAND_SCALE;
	}
	float randSymmetric()
	{
		return float(rand()) * RAND_SCALE * 2.0f - 1.0f;
	}
}}
