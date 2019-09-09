#pragma once
#include "Matrix3.h"
#include "Vector3.h"
#include "Vector4.h"
#include <glm/mat4x4.hpp>
#include <ostream>

namespace Cinnabar
{
	class Matrix4
	{
	public:
		static const Matrix4 IDENTITY;

		union
		{
			float data[16];
			float matrix[4][4];
		};

		Matrix4();
		Matrix4(const float&, const float&, const float&, const float&,
		        const float&, const float&, const float&, const float&,
		        const float&, const float&, const float&, const float&,
		        const float&, const float&, const float&, const float&);
		Matrix4(const Matrix3&);
		Matrix4(const glm::mat4&);

		const float* operator[](const unsigned int&) const;
		float* operator[](const unsigned int&);

		Matrix4& operator+=(const Matrix4&);
		Matrix4& operator-=(const Matrix4&);
		Matrix4 operator+(const Matrix4&) const;
		Matrix4 operator-(const Matrix4&) const;

		Matrix4& operator*=(const float&);
		Matrix4 operator*(const float&) const;

		Matrix4 operator*(const Matrix4&) const;
		Matrix4& operator*=(const Matrix4&);

		Vector4 operator*(const Vector4&) const;

		bool operator==(const Matrix4&) const;
		bool operator!=(const Matrix4&) const;

		void transpose();
		Matrix4 transposed() const;

		static Matrix4 invert(const Matrix4&);

		static Matrix4 translation(const Vector3&);
	};
}

std::ostream& operator<<(std::ostream& os, const Cinnabar::Matrix4& m);
