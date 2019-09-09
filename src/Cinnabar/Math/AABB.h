#pragma once
#include "Vector3.h"

namespace Cinnabar
{
	class AABB
	{
	public:
		AABB();
		AABB(const Vector3&, const Vector3&);

		const Vector3& minimum() const;
		void setMinimum(const Vector3&);

		const Vector3& maximum() const;
		void setMaximum(const Vector3&);

		AABB& operator+=(const AABB&);
		AABB operator+(const AABB&);

		bool contains(const Vector3&);
		bool overlaps(const AABB&);

	private:
		Vector3 _minimum;
		Vector3 _maximum;
	};
}
