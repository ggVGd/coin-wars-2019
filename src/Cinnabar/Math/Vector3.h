#pragma once
#include "Vector2.h"
#include <ostream>

namespace Cinnabar
{
	class Quaternion;

	template<typename DataType>
	class Vector3Base
	{
	public:
		DataType x;
		DataType y;
		DataType z;

		static const Vector3Base<DataType> ZERO;
		static const Vector3Base<DataType> UNIT_X;
		static const Vector3Base<DataType> UNIT_Y;
		static const Vector3Base<DataType> UNIT_Z;

		Vector3Base()
		{
		}
		Vector3Base(const DataType& x, const DataType& y, const DataType& z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		template<typename OtherDataType>
		Vector3Base(const Vector3Base<OtherDataType>& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
		}
		template<typename OtherDataType>
		Vector3Base(const Vector2Base<OtherDataType>& xy, const DataType& z)
		{
			x = xy.x;
			y = xy.y;
			this->z = z;
		}
		DataType operator[](const std::size_t& i) const
		{
			return *(&x+i);
		}
		DataType& operator[](const std::size_t& i)
		{
			return *(&x+i);
		}
		Vector3Base operator+(const DataType& s) const
		{
			return Vector3Base( x + s, y + s, z + s );
		}
		Vector3Base operator-(const DataType& s) const
		{
			return Vector3Base( x - s, y - s, z - s );
		}
		Vector3Base operator/(const DataType& s) const
		{
			return Vector3Base( x / s, y / s, z / s );
		}
		Vector3Base operator*(const DataType& s) const
		{
			return Vector3Base( x * s, y * s, z * s );
		}
		Vector3Base operator+(const Vector3Base& v) const
		{
			return Vector3Base( x + v.x, y + v.y, z + v.z );
		}
		Vector3Base operator-(const Vector3Base& v) const
		{
			return Vector3Base( x - v.x, y - v.y, z - v.z );
		}
		Vector3Base operator/(const Vector3Base& v) const
		{
			return Vector3Base( x / v.x, y / v.y, z / v.z );
		}
		Vector3Base operator*(const Vector3Base& v) const
		{
			return Vector3Base( x * v.x, y * v.y, z * v.z );
		}
		Vector3Base& operator+=(const DataType& s)
		{
			x += s;
			y += s;
			z += s;
			return *this;
		}
		Vector3Base& operator-=(const DataType& s)
		{
			x -= s;
			y -= s;
			z -= s;
			return *this;
		}
		Vector3Base& operator/=(const DataType& s)
		{
			x /= s;
			y /= s;
			z /= s;
			return *this;
		}
		Vector3Base& operator*=(const DataType& s)
		{
			x *= s;
			y *= s;
			z *= s;
			return *this;
		}
		Vector3Base& operator+=(const Vector3Base& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
		Vector3Base& operator-=(const Vector3Base& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
		Vector3Base& operator/=(const Vector3Base& v)
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}
		Vector3Base& operator*=(const Vector3Base& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}
		bool operator<(const Vector3Base& v) const
		{
			return x < v.x && y < v.y && z < v.z;
		}
		bool operator>(const Vector3Base& v) const
		{
			return x > v.x && y > v.y && z > v.z;
		}
		bool operator==(const Vector3Base& v) const
		{
			return x == v.x && y == v.y && z == v.z;
		}
		bool operator!=(const Vector3Base& v) const
		{
			return !operator==(v);
		}
		DataType length2() const
		{
			return x*x + y*y + z*z;
		}
		DataType length() const
		{
			return sqrt(length2());
		}
		void normalize()
		{
			const DataType L = fabs(length());
			x /= L;
			y /= L;
			z /= L;
		}
		Vector3Base normalizedCopy() const
		{
			Vector3Base copy = *this;
			copy.normalize();
			return copy;
		}
		static DataType dot(const Vector3Base& a, const Vector3Base& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}
		DataType dot(const Vector3Base& b) const
		{
			return dot(*this, b);
		}
		static Vector3Base cross(const Vector3Base& a, const Vector3Base& b)
		{
			return Vector3Base(
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
			);
		}
		Vector3Base cross(const Vector3Base& b) const
		{
			return cross(*this, b);
		}
		Quaternion findRotationTo(const Vector3Base& v) const;
	};

	template<typename DataType>
	const Vector3Base<DataType> Vector3Base<DataType>::ZERO(0, 0, 0);

	template<typename DataType>
	const Vector3Base<DataType> Vector3Base<DataType>::UNIT_X(1, 0, 0);

	template<typename DataType>
	const Vector3Base<DataType> Vector3Base<DataType>::UNIT_Y(0, 1, 0);

	template<typename DataType>
	const Vector3Base<DataType> Vector3Base<DataType>::UNIT_Z(0, 0, 1);

	typedef Vector3Base<float> Vector3;
	typedef Vector3Base<int> Vector3i;
}

// The implementation of methods involving Quaternion are defined here because
// of a cyclical dependency between Quaternion and Vector3.
#include "Quaternion.h"
namespace Cinnabar
{
	template<typename DataType>
	Quaternion Vector3Base<DataType>::findRotationTo(const Vector3Base& v) const
	{
		// http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors
		float m = sqrt(2.0f + 2.0f * Vector3Base::dot(*this, v));
		Vector3Base w = (1.0f / m) * Vector3Base::cross(*this, v);
		return Quaternion(0.5f * m, w.x, w.y, w.z);
	}
}

template<typename DataType>
Cinnabar::Vector3Base<DataType> operator+(const DataType& s, const Cinnabar::Vector3Base<DataType>& v)
{
	return v + s;
}
template<typename DataType>
Cinnabar::Vector3Base<DataType> operator-(const DataType& s, const Cinnabar::Vector3Base<DataType>& v)
{
	return v - s;
}
template<typename DataType>
Cinnabar::Vector3Base<DataType> operator/(const DataType& s, const Cinnabar::Vector3Base<DataType>& v)
{
	return v / s;
}
template<typename DataType>
Cinnabar::Vector3Base<DataType> operator*(const DataType& s, const Cinnabar::Vector3Base<DataType>& v)
{
	return v * s;
}

template<typename DataType>
std::ostream& operator<<(std::ostream& os, const Cinnabar::Vector3Base<DataType>& v)
{
	return os << "Vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
}
