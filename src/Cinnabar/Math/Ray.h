#pragma once
#include "Vector3.h"

namespace Cinnabar
{
	class AABB;

	class Ray
	{
	public:
		Ray();
		Ray(const Vector3&, const Vector3&);

		const Vector3& origin() const;
		void setOrigin(const Vector3&);

		const Vector3& direction() const;
		void setDirection(const Vector3&);

		Vector3 point(const float&) const;
		Vector3 operator*(const float&) const;

	private:
		Vector3 _origin;
		Vector3 _direction;
	};
}
