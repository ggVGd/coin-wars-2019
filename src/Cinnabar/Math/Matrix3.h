#pragma once
#include <ostream>

namespace Cinnabar
{
	class Matrix4;
	class Matrix3
	{
	public:
		static const Matrix3 IDENTITY;

		union
		{
			float data[9];
			float matrix[3][3];
		};

		Matrix3();
		Matrix3(const float&, const float&, const float&,
		        const float&, const float&, const float&,
		        const float&, const float&, const float&);
		Matrix3(Matrix4);

		const float* operator[](const unsigned int&) const;
		float* operator[](const unsigned int&);

		Matrix3& operator+=(const Matrix3&);
		Matrix3& operator-=(const Matrix3&);
		Matrix3 operator+(const Matrix3&) const;
		Matrix3 operator-(const Matrix3&) const;

		Matrix3 operator*(const Matrix3&) const;
		Matrix3& operator*=(const Matrix3&);

		bool operator==(const Matrix3&) const;
		bool operator!=(const Matrix3&) const;

		void transpose();
		Matrix3 transposed() const;
	};
}

std::ostream& operator<<(std::ostream& os, const Cinnabar::Matrix3& m);
