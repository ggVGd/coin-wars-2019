#include "Matrix4.h"
#include <cstring>

namespace Cinnabar
{
	const Matrix4 Matrix4::IDENTITY(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	Matrix4::Matrix4()
	{
		memset(data, 0, sizeof(data));
	}
	Matrix4::Matrix4(const float& v00, const float& v10, const float& v20, const float& v30,
	                 const float& v01, const float& v11, const float& v21, const float& v31,
	                 const float& v02, const float& v12, const float& v22, const float& v32,
	                 const float& v03, const float& v13, const float& v23, const float& v33)
	{
		matrix[0][0] = v00;
		matrix[0][1] = v01;
		matrix[0][2] = v02;
		matrix[0][3] = v03;
		matrix[1][0] = v10;
		matrix[1][1] = v11;
		matrix[1][2] = v12;
		matrix[1][3] = v13;
		matrix[2][0] = v20;
		matrix[2][1] = v21;
		matrix[2][2] = v22;
		matrix[2][3] = v23;
		matrix[3][0] = v30;
		matrix[3][1] = v31;
		matrix[3][2] = v32;
		matrix[3][3] = v33;
	}
	Matrix4::Matrix4(const Matrix3& m3)
	{
		matrix[0][0] = m3.matrix[0][0];
		matrix[0][1] = m3.matrix[0][1];
		matrix[0][2] = m3.matrix[0][2];
		matrix[0][3] = 0.0f;
		matrix[1][0] = m3.matrix[1][0];
		matrix[1][1] = m3.matrix[1][1];
		matrix[1][2] = m3.matrix[1][2];
		matrix[1][3] = 0.0f;
		matrix[2][0] = m3.matrix[2][0];
		matrix[2][1] = m3.matrix[2][1];
		matrix[2][2] = m3.matrix[2][2];
		matrix[2][3] = 0.0f;
		matrix[3][0] = 0.0f;
		matrix[3][1] = 0.0f;
		matrix[3][2] = 0.0f;
		matrix[3][3] = 1.0f;
	}
	Matrix4::Matrix4(const glm::mat4& m)
	{
		for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			matrix[i][j] = m[i][j];
		}
	}
	const float* Matrix4::operator[](const unsigned int& y) const
	{
		return &data[y * 4];
	}
	float* Matrix4::operator[](const unsigned int& y)
	{
		return &data[y * 4];
	}
	Matrix4& Matrix4::operator+=(const Matrix4& m)
	{
		for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			matrix[i][j] += m[i][j];
		}
		return *this;
	}
	Matrix4& Matrix4::operator-=(const Matrix4& m)
	{
		for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			matrix[i][j] -= m[i][j];
		}
		return *this;
	}
	Matrix4 Matrix4::operator+(const Matrix4& m) const
	{
		Matrix4 result = *this;
		return result += m;
	}
	Matrix4 Matrix4::operator-(const Matrix4& m) const
	{
		Matrix4 result = *this;
		return result -= m;
	}
	Matrix4& Matrix4::operator*=(const float& s)
	{
		for(int i = 0; i < 16; i++)
		{
			data[i] *= s;
		}
		return *this;
	}
	Matrix4 Matrix4::operator*(const float& s) const
	{
		Matrix4 result = *this;
		result *= s;
		return result;
	}
	Matrix4 Matrix4::operator*(const Matrix4& m) const
	{
		Matrix4 result;
		for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			result[i][j] = 0;
			for(int k = 0; k < 4; k++)
			{
				result[i][j] += matrix[k][j] * m[i][k];
			}
		}
		return result;
	}
	Matrix4& Matrix4::operator*=(const Matrix4& m)
	{
		*this = *this * m;
		return *this;
	}
	Vector4 Matrix4::operator*(const Vector4& v) const
	{
		return Vector4(
			matrix[0][0] * v.x + matrix[1][0] * v.y + matrix[2][0] * v.z + matrix[3][0] * v.w,
			matrix[0][1] * v.x + matrix[1][1] * v.y + matrix[2][1] * v.z + matrix[3][1] * v.w,
			matrix[0][2] * v.x + matrix[1][2] * v.y + matrix[2][2] * v.z + matrix[3][2] * v.w,
			matrix[0][3] * v.x + matrix[1][3] * v.y + matrix[2][3] * v.z + matrix[3][3] * v.w
		);
	}
	bool Matrix4::operator==(const Matrix4& m) const
	{
		for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			if(matrix[i][j] != m[i][j])
				return false;
		}
		return true;
	}
	bool Matrix4::operator!=(const Matrix4& m) const
	{
		return !operator==(m);
	}
	void Matrix4::transpose()
	{
		std::swap(matrix[0][1], matrix[1][0]);
		std::swap(matrix[0][2], matrix[2][0]);
		std::swap(matrix[0][3], matrix[3][0]);
		std::swap(matrix[1][2], matrix[2][1]);
		std::swap(matrix[1][3], matrix[3][1]);
		std::swap(matrix[2][3], matrix[3][2]);
	}
	Matrix4 Matrix4::transposed() const
	{
		Matrix4 result = *this;
		result.transpose();
		return result;
	}
	Matrix4 Matrix4::invert(const Matrix4& in)
	{
		Matrix4 out;
		const float* m = in.data;
		float* inv = out.data;

		inv[0] = m[5]  * m[10] * m[15] -
		         m[5]  * m[11] * m[14] -
		         m[9]  * m[6]  * m[15] +
		         m[9]  * m[7]  * m[14] +
		         m[13] * m[6]  * m[11] -
		         m[13] * m[7]  * m[10];

		inv[4] = -m[4]  * m[10] * m[15] +
		          m[4]  * m[11] * m[14] +
		          m[8]  * m[6]  * m[15] -
		          m[8]  * m[7]  * m[14] -
		          m[12] * m[6]  * m[11] +
		          m[12] * m[7]  * m[10];

		inv[8] = m[4]  * m[9]  * m[15] -
		         m[4]  * m[11] * m[13] -
		         m[8]  * m[5]  * m[15] +
		         m[8]  * m[7]  * m[13] +
		         m[12] * m[5]  * m[11] -
		         m[12] * m[7]  * m[9];

		inv[12] = -m[4]  * m[9]  * m[14] +
		           m[4]  * m[10] * m[13] +
		           m[8]  * m[5]  * m[14] -
		           m[8]  * m[6]  * m[13] -
		           m[12] * m[5]  * m[10] +
		           m[12] * m[6]  * m[9];

		inv[1] = -m[1]  * m[10] * m[15] +
		          m[1]  * m[11] * m[14] +
		          m[9]  * m[2]  * m[15] -
		          m[9]  * m[3]  * m[14] -
		          m[13] * m[2]  * m[11] +
		          m[13] * m[3]  * m[10];

		inv[5] = m[0]  * m[10] * m[15] -
		         m[0]  * m[11] * m[14] -
		         m[8]  * m[2]  * m[15] +
		         m[8]  * m[3]  * m[14] +
		         m[12] * m[2]  * m[11] -
		         m[12] * m[3]  * m[10];

		inv[9] = -m[0]  * m[9]  * m[15] +
		          m[0]  * m[11] * m[13] +
		          m[8]  * m[1]  * m[15] -
		          m[8]  * m[3]  * m[13] -
		          m[12] * m[1]  * m[11] +
		          m[12] * m[3]  * m[9];

		inv[13] = m[0]  * m[9]  * m[14] -
		          m[0]  * m[10] * m[13] -
		          m[8]  * m[1]  * m[14] +
		          m[8]  * m[2]  * m[13] +
		          m[12] * m[1]  * m[10] -
		          m[12] * m[2]  * m[9];

		inv[2] = m[1]  * m[6] * m[15] -
		         m[1]  * m[7] * m[14] -
		         m[5]  * m[2] * m[15] +
		         m[5]  * m[3] * m[14] +
		         m[13] * m[2] * m[7] -
		         m[13] * m[3] * m[6];

		inv[6] = -m[0]  * m[6] * m[15] +
		          m[0]  * m[7] * m[14] +
		          m[4]  * m[2] * m[15] -
		          m[4]  * m[3] * m[14] -
		          m[12] * m[2] * m[7] +
		          m[12] * m[3] * m[6];

		inv[10] = m[0]  * m[5] * m[15] -
		          m[0]  * m[7] * m[13] -
		          m[4]  * m[1] * m[15] +
		          m[4]  * m[3] * m[13] +
		          m[12] * m[1] * m[7] -
		          m[12] * m[3] * m[5];

		inv[14] = -m[0]  * m[5] * m[14] +
		           m[0]  * m[6] * m[13] +
		           m[4]  * m[1] * m[14] -
		           m[4]  * m[2] * m[13] -
		           m[12] * m[1] * m[6] +
		           m[12] * m[2] * m[5];

		inv[3] = -m[1] * m[6] * m[11] +
		          m[1] * m[7] * m[10] +
		          m[5] * m[2] * m[11] -
		          m[5] * m[3] * m[10] -
		          m[9] * m[2] * m[7] +
		          m[9] * m[3] * m[6];

		inv[7] = m[0] * m[6] * m[11] -
		         m[0] * m[7] * m[10] -
		         m[4] * m[2] * m[11] +
		         m[4] * m[3] * m[10] +
		         m[8] * m[2] * m[7] -
		         m[8] * m[3] * m[6];

		inv[11] = -m[0] * m[5] * m[11] +
		           m[0] * m[7] * m[9] +
		           m[4] * m[1] * m[11] -
		           m[4] * m[3] * m[9] -
		           m[8] * m[1] * m[7] +
		           m[8] * m[3] * m[5];

		inv[15] = m[0] * m[5] * m[10] -
		          m[0] * m[6] * m[9] -
		          m[4] * m[1] * m[10] +
		          m[4] * m[2] * m[9] +
		          m[8] * m[1] * m[6] -
		          m[8] * m[2] * m[5];

		float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		// If the determinant is 0, no inverse exists.
		if(det == 0)
		{
			// Is there a better approach than returning zero?
			return Matrix4(
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0
			);
		}

		det = 1.0 / det;
		return out * det;
	}
	Matrix4 Matrix4::translation(const Vector3& v)
	{
		return Matrix4(
			1, 0, 0, v.x,
			0, 1, 0, v.y,
			0, 0, 1, v.z,
			0, 0, 0, 1
		);
	}
}


std::ostream& operator<<(std::ostream& os, const Cinnabar::Matrix4& m)
{
	return os << "Matrix4("
		<< "[" << m[0][0] << "," << m[1][0] << "," << m[2][0] << "," << m[3][0] << "],"
		<< "[" << m[0][1] << "," << m[1][1] << "," << m[2][1] << "," << m[3][1] << "],"
		<< "[" << m[0][2] << "," << m[1][2] << "," << m[2][2] << "," << m[3][2] << "],"
		<< "[" << m[0][3] << "," << m[1][3] << "," << m[2][3] << "," << m[3][3] << "]"
		<< ")";
}
