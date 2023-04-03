#include "mymath.h"

const Vector2 Vector2::Zero(0.0f, 0.0f);
const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::Forward(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Infinity(math::Inifinity, math::Inifinity, math::Inifinity);

Vector3 Vector3::Rotate(const Vector3& v, const Quaternion& q)
{
	Vector3 qv(q.x, q.y, q.z);
	Vector3 result = v + 2.0f * Vector3::Cross(qv, Vector3::Cross(qv, v) + q.w * v);
	return result;
}

const Quaternion Quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);