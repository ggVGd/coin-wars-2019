#include "Quaternion.h"
#include "Constants.h"
#include "Math.h"
#include "Matrix4.h"
#include "Vector3.h"
#include <cmath>

namespace Cinnabar
{
	Quaternion::Quaternion()
	{
		w = 1;
		x = 0;
		y = 0;
		z = 0;
	}
	Quaternion::Quaternion(const float& w, const float& x, const float& y, const float& z)
	{
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Quaternion Quaternion::operator+(const Quaternion& q) const
	{
		return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
	}
	Quaternion Quaternion::operator-(const Quaternion& q) const
	{
		return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
	}
	Quaternion& Quaternion::operator+=(const Quaternion& q)
	{
		w += q.w;
		x += q.x;
		y += q.y;
		z += q.z;
		return *this;
	}
	Quaternion& Quaternion::operator-=(const Quaternion& q)
	{
		w += q.w;
		x += q.x;
		y += q.y;
		z += q.z;
		return *this;
	}
	Quaternion Quaternion::operator*(const Quaternion& q) const
	{
		return Quaternion(
			w * q.w - x * q.x - y * q.y - z * q.z,
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y + y * q.w + z * q.x - x * q.z,
			w * q.z + z * q.w + x * q.y - y * q.x
		);
	}
	Quaternion Quaternion::operator*(const float& s) const
	{
		return Quaternion(s*w, s*x, s*y, s*z);
	}
	Quaternion& Quaternion::operator*=(const Quaternion& q)
	{
		*this = operator*(q);
		return *this;
	}
	Quaternion& Quaternion::operator*=(const float& s)
	{
		*this = operator*(s);
		return *this;
	}
	Vector3 Quaternion::operator*(const Vector3& v) const
	{
		//const Vector3 u(x, y, z);
		//const float s = w;
		//return 2.0f * Cinnabar::Vector3::dot(u, v) * u
		//	+ (s*s - Cinnabar::Vector3::dot(u, u)) * v
		//	+ 2.0f * s * Cinnabar::Vector3::cross(u, v);

		Vector3 qvec(x, y, z);
		Vector3 uv = Vector3::cross(qvec, v);
		Vector3 uuv = Vector3::cross(qvec, uv);
		uv *= (2.0f * w);
		uuv *= 2.0f;
		return v + uv + uuv;
	}
	void Quaternion::normalize()
	{
		const float n = sqrt((w*w) + (x*x) + (y*y) + (z*z));
		w /= n;
		x /= n;
		y /= n;
		z /= n;
	}
	Quaternion Quaternion::normalized() const
	{
		Quaternion result = *this;
		result.normalize();
		return result;
	}
	Quaternion Quaternion::fromAngleAxis(const float& angle, const Vector3 axis)
	{
		const float s = sin(angle * 0.5f);
		return Quaternion(
			cos(angle * 0.5f),
			axis.x * s,
			axis.y * s,
			axis.z * s
		);
	}
	Quaternion Quaternion::fromEulerAngles(const float& pitch, const float& yaw, const float& roll)
	{
		const float cy = cos(yaw * 0.5);
		const float sy = sin(yaw * 0.5);
		const float cr = cos(roll * 0.5);
		const float sr = sin(roll * 0.5);
		const float cp = cos(pitch * 0.5);
		const float sp = sin(pitch * 0.5);
		return Quaternion(
			cy * cr * cp + sy * sr * sp,
			cy * sr * cp - sy * cr * sp,
			cy * cr * sp + sy * sr * cp,
			sy * cr * cp - cy * sr * sp
		);
	}
	void Quaternion::toEulerAngles(float& pitch, float& yaw, float& roll)
	{
		// roll (x-axis rotation)
		const float sinr_cosp = 2.0f * (w * x + y * z);
		const float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
		roll = atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		const float sinp = 2.0f * (w * y - z * x);
		if(fabs(sinp) >= 1.0f)
			pitch = copysign(pi() / 2.0f, sinp);
		else
			pitch = asin(sinp);

		// yaw (z-axis rotation)
		const float siny_cosp = 2.0f * (w * z + x * y);
		const float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
		yaw = atan2(siny_cosp, cosy_cosp);
	}
	Matrix3 Quaternion::toRotationMatrix() const
	{
		const Quaternion q = normalized();
		const float w = q.w;
		const float x = q.x;
		const float y = q.y;
		const float z = q.z;
		const float w2 = q.w*q.w;
		const float x2 = q.x*q.x;
		const float y2 = q.y*q.y;
		const float z2 = q.z*q.z;
		return Matrix3(
			1-2*y2-2*z2, 2*x*y-2*z*w, 2*x*z+2*y*w,
			2*x*y+2*z*w, 1-2*x2-2*z2, 2*y*z-2*x*w,
			2*x*z-2*y*w, 2*y*z+2*x*w, 1-2*x2-2*y2
		);
	}
	float Quaternion::dot(Quaternion A, Quaternion B)
	{
		return A.w*B.w + A.x*B.x + A.y*B.y + A.z*B.z;
	}
	Quaternion Quaternion::slerp(Quaternion A, Quaternion B, const float& t)
	{
		A.normalize();
		B.normalize();

		float dot = Quaternion::dot(A, B);

		if(dot < 0.0f)
		{
			A = Quaternion(-A.w, -A.x, -A.y, -A.z);
			dot = -dot;
		}

		const float DOT_THRESHOLD = 0.9995f;
		if(dot > DOT_THRESHOLD)
		{
			Quaternion result = A + (B - A) * t;
			result.normalize();
			return result;
		}

		dot = clamp(dot, -1.0f, 1.0f);
		float theta_0 = acos(dot);
		float theta = theta_0*t;
		float s0 = cos(theta) - dot * sin(theta) / sin(theta_0);
		float s1 = sin(theta) / sin(theta_0);
		return (A * s0) + (B * s1);
	}
}

std::ostream& operator<<(std::ostream& os, const Cinnabar::Quaternion& q)
{
	return os << "Quaternion(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
}
