#include "Ray.h"
#include "AABB.h"

namespace Cinnabar
{
	Ray::Ray()
	{
		_origin = Vector3::ZERO;
		_direction = Vector3::UNIT_X;
	}
	Ray::Ray(const Vector3& origin, const Vector3& direction)
	{
		setOrigin(origin);
		setDirection(direction);
	}
	const Vector3& Ray::origin() const
	{
		return _origin;
	}
	void Ray::setOrigin(const Vector3& origin)
	{
		_origin = origin;
	}
	const Vector3& Ray::direction() const
	{
		return _direction;
	}
	void Ray::setDirection(const Vector3& direction)
	{
		_direction = direction.normalizedCopy();
	}
	Vector3 Ray::point(const float& x) const
	{
		return _origin + _direction * x;
	}
	Vector3 Ray::operator*(const float& x) const
	{
		return point(x);
	}
}
