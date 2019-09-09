#include "AABB.h"
#include "Math.h"

namespace Cinnabar
{
	AABB::AABB()
	{
	}
	AABB::AABB(const Vector3& minimum, const Vector3& maximum)
		: _minimum(minimum), _maximum(maximum)
	{
	}
	const Vector3& AABB::minimum() const
	{
		return _minimum;
	}
	void AABB::setMinimum(const Vector3& minimum)
	{
		_minimum = minimum;
	}
	const Vector3& AABB::maximum() const
	{
		return _maximum;
	}
	void AABB::setMaximum(const Vector3& maximum)
	{
		_maximum = maximum;
	}
	AABB& AABB::operator+=(const AABB& other)
	{
		_minimum.x = min(_minimum.x, other._minimum.x);
		_minimum.y = min(_minimum.y, other._minimum.y);
		_minimum.z = min(_minimum.z, other._minimum.z);
		_maximum.x = max(_maximum.x, other._maximum.x);
		_maximum.y = max(_maximum.y, other._maximum.y);
		_maximum.z = max(_maximum.z, other._maximum.z);
		return *this;
	}
	AABB AABB::operator+(const AABB& other)
	{
		AABB result = *this;
		result += other;
		return result;
	}
	bool AABB::contains(const Vector3& point)
	{
		return (
			(_minimum.x <= point.x && point.x <= _maximum.x) &&
			(_minimum.y <= point.y && point.y <= _maximum.y) &&
			(_minimum.z <= point.z && point.z <= _maximum.z)
		);
	}
	bool AABB::overlaps(const AABB& other)
	{
		return (
			(_minimum.x <= other._maximum.x && _maximum.x >= other._minimum.x) &&
			(_minimum.y <= other._maximum.y && _maximum.y >= other._minimum.y) &&
			(_minimum.z <= other._maximum.z && _maximum.z >= other._minimum.z)
		);
	}
}
