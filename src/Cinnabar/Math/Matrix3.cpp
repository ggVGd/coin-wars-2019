#include "Matrix3.h"
#include "Matrix4.h"
#include <cstring>

namespace Cinnabar
{
	const Matrix3 Matrix3::IDENTITY(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	);

	Matrix3::Matrix3()
	{
		memset(data, 0, sizeof(data));
	}
	Matrix3::Matrix3(const float& v00, const float& v10, const float& v20,
	                 const float& v01, const float& v11, const float& v21,
	                 const float& v02, const float& v12, const float& v22)
	{
		matrix[0][0] = v00;
		matrix[0][1] = v01;
		matrix[0][2] = v02;
		matrix[1][0] = v10;
		matrix[1][1] = v11;
		matrix[1][2] = v12;
		matrix[2][0] = v20;
		matrix[2][1] = v21;
		matrix[2][2] = v22;
	}
	Matrix3::Matrix3(Matrix4 m4)
	{
		matrix[0][0] = m4.matrix[0][0];
		matrix[0][1] = m4.matrix[0][1];
		matrix[0][2] = m4.matrix[0][2];
		matrix[1][0] = m4.matrix[1][0];
		matrix[1][1] = m4.matrix[1][1];
		matrix[1][2] = m4.matrix[1][2];
		matrix[2][0] = m4.matrix[2][0];
		matrix[2][1] = m4.matrix[2][1];
		matrix[2][2] = m4.matrix[2][2];
	}
	const float* Matrix3::operator[](const unsigned int& y) const
	{
		return &data[y * 3];
	}
	float* Matrix3::operator[](const unsigned int& y)
	{
		return &data[y * 3];
	}
	Matrix3& Matrix3::operator+=(const Matrix3& m)
	{
		for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
		{
			matrix[i][j] += m[i][j];
		}
		return *this;
	}
	Matrix3& Matrix3::operator-=(const Matrix3& m)
	{
		for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
		{
			matrix[i][j] -= m[i][j];
		}
		return *this;
	}
	Matrix3 Matrix3::operator+(const Matrix3& m) const
	{
		Matrix3 result = *this;
		return result += m;
	}
	Matrix3 Matrix3::operator-(const Matrix3& m) const
	{
		Matrix3 result = *this;
		return result -= m;
	}
	Matrix3 Matrix3::operator*(const Matrix3& m) const
	{
		Matrix3 result;
		for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
		{
			result[i][j] = 0;
			for(int k = 0; k < 3; k++)
			{
				result[i][j] += matrix[k][j] * m[i][k];
			}
		}
		return result;
	}
	Matrix3& Matrix3::operator*=(const Matrix3& m)
	{
		*this = *this + m;
		return *this;
	}
	bool Matrix3::operator==(const Matrix3& m) const
	{
		for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
		{
			if(matrix[i][j] != m[i][j])
				return false;
		}
		return true;
	}
	bool Matrix3::operator!=(const Matrix3& m) const
	{
		return !operator==(m);
	}
	void Matrix3::transpose()
	{
		std::swap(matrix[0][1], matrix[1][0]);
		std::swap(matrix[0][2], matrix[2][0]);
		std::swap(matrix[1][2], matrix[2][1]);
	}
	Matrix3 Matrix3::transposed() const
	{
		Matrix3 result = *this;
		result.transpose();
		return result;
	}
}


std::ostream& operator<<(std::ostream& os, const Cinnabar::Matrix3& m)
{
	return os << "Matrix3("
		<< "[" << m[0][0] << "," << m[1][0] << "," << m[2][0] << "],"
		<< "[" << m[0][1] << "," << m[1][1] << "," << m[2][1] << "],"
		<< "[" << m[0][2] << "," << m[1][2] << "," << m[2][2] << "]"
		<< ")";
}
