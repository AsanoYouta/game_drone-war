#pragma once

#include "framework.h"
#include <cmath>
#include <limits.h>
#include <DirectXMath.h>
using namespace DirectX;

#define PI (3.1415926535f)

namespace math
{
	const float Inifinity = std::numeric_limits<float>::infinity();

	inline float ToRadian(float angle)
	{
		return angle * PI / 180.0f;
	}

	inline float ToDegree(float radian)
	{
		return 180.0f / PI * radian;
	}

	inline float Lerp(float a, float b, float f)
	{
		return a + (b - a) * f;
	}

	inline bool NearZero(float val, float epsilon = 0.001f)
	{
		if (fabs(val) <= epsilon) return true;
		else return false;
	}

	//ガウスブラーの重みを計算する
	inline void SetGausWeights(float outWeights[16], float disp)
	{
		float sum = 0;
		for (int i = 0; i < 16; i++)
		{
			float x = float(i);
			outWeights[i] = expf(-(x * x) / (disp * 2.0f));
			if (i != 0) outWeights[i] *= 2.0f;
			sum += outWeights[i];
		}
		//weightsの合計が１になるように
		for (int i = 0; i < 16; i++) outWeights[i] /= sum;
	}

	//floatを時刻表記のワイド文字列に変換する(00:00:00の形)
	inline std::wstring ConvertToTimeRotation(float time)
	{
		float minval = time / 60.0f;
		float secval = (minval - int(minval)) * 60.0f;
		auto minutes = std::to_wstring(int(minval));
		auto seconds = std::to_wstring(int(secval));
		auto decimal = std::to_wstring(int((time - int(time)) * 100));
		if (minutes.size() == 1) minutes.insert(0, L"0");
		if (seconds.size() == 1) seconds.insert(0, L"0");
		if (decimal.size() == 1) decimal.insert(0, L"0");
		return (minutes + L":" + seconds + L":" + decimal);
	}
}

class Vector2
{
public:
	float x, y;

	Vector2()
		:x(0.0f)
		,y(0.0f)
	{}
	explicit Vector2(float x, float y)
		:x(x)
		,y(y)
	{}

	friend Vector2 operator+(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x + b.x, a.y + b.y);
	}

	friend Vector2 operator-(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x - b.x, a.y - b.y);
	}

	friend Vector2 operator-(const Vector2& a)
	{
		return Vector2(-a.x, -a.y);
	}

	friend Vector2 operator*(float scalar, const Vector2& v)
	{
		return Vector2(v.x * scalar, v.y * scalar);
	}
	friend Vector2 operator*(const Vector2& v, float scalar)
	{
		return Vector2(v.x * scalar, v.y * scalar);
	}

	Vector2& operator*= (float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	float LengthSq() const //高価な平方根演算を避けるため
	{
		return x * x + y * y;
	}

	float Length() const
	{
		return sqrtf(x * x + y * y);
	}

	void Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;
	}

	Vector2 Normalized() const
	{
		float length = Length();
		Vector2 n(x, y);
		n.x /= length;
		n.y /= length;
		return n;
	}

	static float Distance(Vector2 a, Vector2 b)
	{
		return fabs(a.x - b.x) + fabs(a.y - b.y);
	}

	static float Dot(const Vector2& a, const Vector2& b)
	{
		return(a.x * b.x + a.y * b.y);
	}

	static Vector2 Lerp(const Vector2& a, const Vector2& b, float f)
	{
		return Vector2(a + (b - a) * f);
	}

	static const Vector2 Zero;
};

class Vector3
{
public:
	float x, y, z;

	Vector3()
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
	{}
	explicit Vector3(float x, float y, float z)
		: x(x)
		, y(y)
		, z(z)
	{}

	friend Vector3 operator+(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	friend Vector3 operator-(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	friend Vector3 operator-(const Vector3& a)
	{
		return Vector3(-a.x, -a.y, -a.z);
	}

	friend Vector3 operator*(float scalar, const Vector3& v)
	{
		return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
	}
	friend Vector3 operator*(const Vector3& v, float scalar)
	{
		return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
	}

	friend Vector3 operator/(const Vector3& v, float scalar)
	{
		return Vector3(v.x / scalar, v.y / scalar, v.z / scalar);
	}
	friend Vector3 operator/(float scalar, const Vector3& v)
	{
		return Vector3(scalar/v.x, scalar/v.y, scalar/v.z);
	}


	Vector3& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	Vector3& operator+=(const Vector3& right)
	{
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& right)
	{
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	Vector3& operator/=(const float& scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	bool IsZero()
	{
		if (x == 0 && y == 0 && z == 0) return true;
		else return false;
	}

	float LengthSq() const //高価な平方根演算を避けるため
	{
		return x * x + y * y + z * z;
	}

	float Length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	void Normalize()
	{
		float length = Length();
		if (length == 0) return;
		x /= length;
		y /= length;
		z /= length;
	}

	Vector3 Normalized() const
	{
		float length = Length();
		if (length == 0) return Vector3::Zero;

		Vector3 n(x,y,z);
		n.x /= length;
		n.y /= length;
		n.z /= length;
		return n;
	}

	static Vector3 Normalized(const Vector3& v)
	{
		float length = v.Length();
		if (length == 0) return Vector3::Zero;

		Vector3 n(v.x, v.y, v.z);
		n.x /= length;
		n.y /= length;
		n.z /= length;
		return n;
	}

	static float Distance(const Vector3& a, const Vector3& b)
	{
		auto temp = a - b;
		return temp.Length();
	}

	static float DistanceSq(const Vector3& a, const Vector3& b)
	{
		auto temp = a - b;
		return temp.LengthSq();
	}

	static float Dot(const Vector3& a, const Vector3& b)
	{
		return(a.x * b.x + a.y * b.y + a.z * b.z);
	}

	static Vector3 Lerp(const Vector3& a, const Vector3& b, float f)
	{
		return Vector3(a + (b - a) * f);
	}

	//a×b
	static Vector3 Cross(const Vector3& a, const Vector3& b)
	{
		Vector3 temp;
		temp.x = a.y * b.z - a.z * b.y;
		temp.y = a.z * b.x - a.x * b.z;
		temp.z = a.x * b.y - a.y * b.x;
		return temp;
	}

	//法線nに対する反射
	static Vector3 Reflect(const Vector3& v, const Vector3& n)
	{
		return v - 2.0f * Vector3::Dot(v, n) * n;
	}

	//ベクトルをクォータニオンで回転
	static Vector3 Rotate(const Vector3& v, const class Quaternion& q);

	static const Vector3 Zero;
	static const Vector3 Forward;
	static const Vector3 Up;
	static const Vector3 Right;
	static const Vector3 Infinity;
};

class Quaternion
{
public:
	float x, y, z, w;

	Quaternion()
	{
		*this = Quaternion::Identity;
	}
	explicit Quaternion(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) 
	{}
	
	void Set(float inX, float inY, float inZ, float inW)
	{
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}

	//軸にradian分回転するクォータニオンを構築(軸は単位ベクトル、角度はラジアン)
	explicit Quaternion(const Vector3& axis, float radian)
	{
		float scalar = sinf(radian / 2.0f);
		x = axis.x * scalar;
		y = axis.y * scalar;
		z = axis.z * scalar;
		w = cosf(radian / 2.0f);
	}

	void Conjugate()
	{
		x *= -1.0f;
		y *= -1.0f;
		z *= -1.0f;
	}

	float Length() const
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	void Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	static float Dot(const Quaternion& a, const Quaternion& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	//連結(先にq, 次にpで回転. グラスマン積pqを使用)
	static Quaternion Concatenate(const Quaternion& q, const Quaternion& p)
	{
		Vector3 qv(q.x, q.y, q.z);
		Vector3 pv(p.x, p.y, p.z);
		Vector3 rv = qv * p.w + q.w * pv + Vector3::Cross(pv, qv);
		float rs = p.w * q.w - Vector3::Dot(pv, qv);

		return Quaternion(rv.x, rv.y, rv.z, rs);
	}

	static const Quaternion Identity;

	//行列への変換はXMMatrixRotationQuaternion()を用いる
};

static XMVECTOR ConvertToXMVECTOR(const Quaternion& q)
{
	return XMVectorSet(q.x, q.y, q.z, q.w);
}
static XMVECTOR ConvertToXMVECTOR(const Vector3& v)
{
	return XMVectorSet(v.x, v.y, v.z, 1.0f);
}
