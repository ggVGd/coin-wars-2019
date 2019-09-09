#pragma once
#include "Matrix3.h"

namespace Cinnabar
{
	template<typename DataType>
	class Vector3Base;

	typedef Vector3Base<float> Vector3;

	class Quaternion
	{
	public:
		float w;
		float x;
		float y;
		float z;

		Quaternion();
		Quaternion(const float&, const float&, const float&, const float&);

		Quaternion operator+(const Quaternion&) const;
		Quaternion operator-(const Quaternion&) const;
		Quaternion& operator+=(const Quaternion&);
		Quaternion& operator-=(const Quaternion&);

		Quaternion operator*(const Quaternion&) const;
		Quaternion operator*(const float&) const;
		Quaternion& operator*=(const Quaternion&);
		Quaternion& operator*=(const float&);
		Vector3 operator*(const Vector3&) const;

		void normalize();
		Quaternion normalized() const;

		static Quaternion fromAngleAxis(const float&, const Vector3);

		static Quaternion fromEulerAngles(const float& pitch, const float& yaw, const float& roll);
		void toEulerAngles(float& pitch, float& yaw, float& roll);

		Matrix3 toRotationMatrix() const;

		static float dot(Quaternion A, Quaternion B);
		static Quaternion slerp(Quaternion A, Quaternion B, const float& t);
	};
}

std::ostream& operator<<(std::ostream& os, const Cinnabar::Quaternion& q);
