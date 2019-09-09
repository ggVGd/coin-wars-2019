#pragma once
#include <cmath>
#include <ostream>

namespace Cinnabar
{
	template<typename DataType>
	class Vector2Base
	{
	public:
		DataType x;
		DataType y;

		static const Vector2Base<DataType> ZERO;
		static const Vector2Base<DataType> UNIT_X;
		static const Vector2Base<DataType> UNIT_Y;

		Vector2Base()
		{
		}
		Vector2Base(const DataType& x, const DataType& y)
		{
			this->x = x;
			this->y = y;
		}
		template<typename OtherDataType>
		Vector2Base(const Vector2Base<OtherDataType>& v)
		{
			x = v.x;
			y = v.y;
		}
		DataType operator[](const std::size_t& i) const
		{
			return *(&x+i);
		}
		DataType& operator[](const std::size_t& i)
		{
			return *(&x+i);
		}
		Vector2Base operator+(const DataType& s) const
		{
			return Vector2Base( x + s, y + s );
		}
		Vector2Base operator-(const DataType& s) const
		{
			return Vector2Base( x - s, y - s );
		}
		Vector2Base operator/(const DataType& s) const
		{
			return Vector2Base( x / s, y / s );
		}
		Vector2Base operator*(const DataType& s) const
		{
			return Vector2Base( x * s, y * s );
		}
		Vector2Base operator+(const Vector2Base& v) const
		{
			return Vector2Base( x + v.x, y + v.y );
		}
		Vector2Base operator-(const Vector2Base& v) const
		{
			return Vector2Base( x - v.x, y - v.y );
		}
		Vector2Base operator/(const Vector2Base& v) const
		{
			return Vector2Base( x / v.x, y / v.y );
		}
		Vector2Base operator*(const Vector2Base& v) const
		{
			return Vector2Base( x * v.x, y * v.y );
		}
		Vector2Base& operator+=(const DataType& s)
		{
			x += s;
			y += s;
			return *this;
		}
		Vector2Base& operator-=(const DataType& s)
		{
			x -= s;
			y -= s;
			return *this;
		}
		Vector2Base& operator/=(const DataType& s)
		{
			x /= s;
			y /= s;
			return *this;
		}
		Vector2Base& operator*=(const DataType& s)
		{
			x *= s;
			y *= s;
			return *this;
		}
		Vector2Base& operator+=(const Vector2Base& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		Vector2Base& operator-=(const Vector2Base& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}
		Vector2Base& operator/=(const Vector2Base& v)
		{
			x /= v.x;
			y /= v.y;
			return *this;
		}
		Vector2Base& operator*=(const Vector2Base& v)
		{
			x *= v.x;
			y *= v.y;
			return *this;
		}
		bool operator<(const Vector2Base& v) const
		{
			return x < v.x && y < v.y;
		}
		bool operator>(const Vector2Base& v) const
		{
			return x > v.x && y > v.y;
		}
		bool operator==(const Vector2Base& v) const
		{
			return x == v.x && y == v.y;
		}
		bool operator!=(const Vector2Base& v) const
		{
			return !operator==(v);
		}
		DataType length2() const
		{
			return x*x + y*y;
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
		}
		Vector2Base normalizedCopy() const
		{
			Vector2Base copy = *this;
			copy.normalize();
			return copy;
		}
		static DataType dot(const Vector2Base& a, const Vector2Base& b)
		{
			return a.x * b.x + a.y * b.y;
		}
		DataType dot(const Vector2Base& b) const
		{
			return dot(*this, b);
		}

		// For use with std::set
		struct KeyCompare
		{
			bool operator()(const Vector2Base& a, const Vector2Base& b)const
			{
				return a.x < b.x || (a.x == b.x && a.y < b.y);
			}
		};

		// For use with std::unordered_map
		struct Hasher
		{
			std::size_t operator()(const Vector2Base a) const
			{
				return std::hash<int>()(a.x) & std::hash<int>()(a.y);
			}
			bool operator()(const Vector2Base& a, const Vector2Base& b) const
			{
				return a.x == b.x && a.y == b.y;
			}
		};
	};

	template<typename DataType>
	const Vector2Base<DataType> Vector2Base<DataType>::ZERO(0, 0);

	template<typename DataType>
	const Vector2Base<DataType> Vector2Base<DataType>::UNIT_X(1, 0);

	template<typename DataType>
	const Vector2Base<DataType> Vector2Base<DataType>::UNIT_Y(0, 1);

	typedef Vector2Base<float> Vector2;
	typedef Vector2Base<int> Vector2i;
}

template<typename DataType>
Cinnabar::Vector2Base<DataType> operator+(const DataType& s, const Cinnabar::Vector2Base<DataType>& v)
{
	return v + s;
}
template<typename DataType>
Cinnabar::Vector2Base<DataType> operator-(const DataType& s, const Cinnabar::Vector2Base<DataType>& v)
{
	return v - s;
}
template<typename DataType>
Cinnabar::Vector2Base<DataType> operator/(const DataType& s, const Cinnabar::Vector2Base<DataType>& v)
{
	return v / s;
}
template<typename DataType>
Cinnabar::Vector2Base<DataType> operator*(const DataType& s, const Cinnabar::Vector2Base<DataType>& v)
{
	return v * s;
}

template<typename DataType>
std::ostream& operator<<(std::ostream& os, const Cinnabar::Vector2Base<DataType>& v)
{
	return os << "Vector2(" << v.x << ", " << v.y << ")";
}

namespace std
{
	template<typename DataType>
	struct hash<Cinnabar::Vector2Base<DataType>>
	{
		std::size_t operator()(const Cinnabar::Vector2Base<DataType>& v) const
		{
			return hash<DataType>()(v.x) ^ hash<DataType>()(v.y);
		}
	};
}
