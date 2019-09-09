#pragma once
#include "Vector3.h"
#include <ostream>

namespace Cinnabar
{
	template<typename DataType>
	class Vector4Base
	{
	public:
		union { DataType x; DataType r; };
		union { DataType y; DataType g; };
		union { DataType z; DataType b; };
		union { DataType w; DataType a; };

		static const Vector4Base<DataType> ZERO;

		Vector4Base()
		{
		}
		Vector4Base(const DataType& x, const DataType& y, const DataType& z, const DataType& w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
		template<typename OtherDataType>
		Vector4Base(const Vector4Base<OtherDataType>& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
		}
		template<typename OtherDataType>
		Vector4Base(const Vector3Base<OtherDataType>& v, const DataType& w = 0)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			this->w = w;
		}
		DataType operator[](const std::size_t& i) const
		{
			return *(&x+i);
		}
		DataType& operator[](const std::size_t& i)
		{
			return *(&x+i);
		}
		Vector4Base operator+(const DataType& s) const
		{
			return Vector4Base( x + s, y + s, z + s, w + s );
		}
		Vector4Base operator-(const DataType& s) const
		{
			return Vector4Base( x - s, y - s, z - s, w - s );
		}
		Vector4Base operator/(const DataType& s) const
		{
			return Vector4Base( x / s, y / s, z / s, w / s );
		}
		Vector4Base operator*(const DataType& s) const
		{
			return Vector4Base( x * s, y * s, z * s, w * s );
		}
		Vector4Base operator+(const Vector4Base& v) const
		{
			return Vector4Base( x + v.x, y + v.y, z + v.z, w + v.w );
		}
		Vector4Base operator-(const Vector4Base& v) const
		{
			return Vector4Base( x - v.x, y - v.y, z - v.z, w - v.w );
		}
		Vector4Base operator/(const Vector4Base& v) const
		{
			return Vector4Base( x / v.x, y / v.y, z / v.z, w / v.w );
		}
		Vector4Base operator*(const Vector4Base& v) const
		{
			return Vector4Base( x * v.x, y * v.y, z * v.z, w * v.w );
		}
		Vector4Base& operator+=(const DataType& s)
		{
			x += s;
			y += s;
			z += s;
			w += s;
			return *this;
		}
		Vector4Base& operator-=(const DataType& s)
		{
			x -= s;
			y -= s;
			z -= s;
			w -= s;
			return *this;
		}
		Vector4Base& operator/=(const DataType& s)
		{
			x /= s;
			y /= s;
			z /= s;
			w /= s;
			return *this;
		}
		Vector4Base& operator*=(const DataType& s)
		{
			x *= s;
			y *= s;
			z *= s;
			w *= s;
			return *this;
		}
		Vector4Base& operator+=(const Vector4Base& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}
		Vector4Base& operator-=(const Vector4Base& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}
		Vector4Base& operator/=(const Vector4Base& v)
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
			w /= v.w;
			return *this;
		}
		Vector4Base& operator*=(const Vector4Base& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			w *= v.w;
			return *this;
		}
		bool operator<(const Vector4Base& v) const
		{
			return x < v.x && y < v.y && z < v.z && w < v.w;
		}
		bool operator>(const Vector4Base& v) const
		{
			return x > v.x && y > v.y && z > v.z && w < v.w;
		}
		bool operator==(const Vector4Base& v) const
		{
			return x == v.x && y == v.y && z == v.z && w == v.w;
		}
		bool operator!=(const Vector4Base& v) const
		{
			return !operator==(v);
		}
		DataType length2() const
		{
			return x*x + y*y + z*z + w*w;
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
			w /= L;
		}
		Vector4Base normalizedCopy() const
		{
			Vector4Base copy = *this;
			copy.normalize();
			return copy;
		}
		DataType dot(const Vector4Base& a, const Vector4Base& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}
		DataType dot(const Vector4Base& b) const
		{
			return dot(*this, b);
		}
	};

	template<typename DataType>
	const Vector4Base<DataType> Vector4Base<DataType>::ZERO(0, 0, 0, 0);

	typedef Vector4Base<float> Vector4;
	typedef Vector4Base<int> Vector4i;
}

template<typename DataType>
Cinnabar::Vector4Base<DataType> operator+(const DataType& s, const Cinnabar::Vector4Base<DataType>& v)
{
	return v + s;
}
template<typename DataType>
Cinnabar::Vector4Base<DataType> operator-(const DataType& s, const Cinnabar::Vector4Base<DataType>& v)
{
	return v - s;
}
template<typename DataType>
Cinnabar::Vector4Base<DataType> operator/(const DataType& s, const Cinnabar::Vector4Base<DataType>& v)
{
	return v / s;
}
template<typename DataType>
Cinnabar::Vector4Base<DataType> operator*(const DataType& s, const Cinnabar::Vector4Base<DataType>& v)
{
	return v * s;
}

template<typename DataType>
std::ostream& operator<<(std::ostream& os, const Cinnabar::Vector4Base<DataType>& v)
{
	return os << "Vector4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
}
