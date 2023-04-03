#include "MoveComponent2.h"
#include "GameObject.h"
#include "Source/Math/mymath.h"

MoveComponent2::MoveComponent2(class GameObject* owner, int updateOrder)
	:Component(owner, updateOrder)
	, m_useGravity(true)
	, m_accel(Vector3::Zero)
	, m_addAccel(Vector3::Zero)
	, m_velocity(Vector3::Zero)
	, m_exceptionVel(Vector3::Zero)
	, m_moveAmount(Vector3::Zero)
	, m_angularSpeed(0.0f)
	, m_maxSpeed(150.0f)
	, m_maxAccel(400.0f)
	, m_brake(180.0f)
{
}

void MoveComponent2::Update(float deltaTime)
{
	//y軸回転
	if (!math::NearZero(m_angularSpeed))
	{
		Quaternion rot = m_owner->GetRotation();
		float angle = m_angularSpeed * deltaTime;
		Quaternion inc(Vector3::Up, angle);
		rot = Quaternion::Concatenate(rot, inc);
		m_owner->SetRotation(rot);
	}

	//疑似摩擦
	m_accel += -m_velocity.Normalized() * m_brake;
	//重力加速度/速度更新
	auto gAccel = -Vector3::Up * 9.8f * 50.0f;
	if (m_useGravity) {
		m_gVelocity += (gAccel-m_gVelocity*0.35f) * deltaTime;
	}
	else {
		m_gVelocity = Vector3::Zero;
	}
	

	//速度更新
	m_velocity += m_accel * deltaTime;
	LimitVelocity();
	if (m_velocity.LengthSq() < 5.0f) {
		m_velocity = Vector3::Zero;
	}
	//制限がない速度の更新
	m_exceptionVel += m_addAccel * deltaTime;
	auto resistAccel = -m_exceptionVel * 3.5f; //疑似空気抵抗
	m_exceptionVel += resistAccel * deltaTime;
	if (m_exceptionVel.LengthSq() < 5.0f) {
		m_exceptionVel = Vector3::Zero;
	}
	//瞬間加速度初期化
	m_addAccel = Vector3::Zero;

	//位置更新
	Vector3 pos = m_owner->GetPosition();
	m_moveAmount = m_velocity * deltaTime;
	m_moveAmount += m_exceptionVel * deltaTime;
	m_moveAmount += m_gVelocity * deltaTime;
	pos += m_moveAmount;
	m_owner->SetPosition(pos);
}

void MoveComponent2::LimitVelocity()
{
	float lenSq = m_velocity.LengthSq();
	float maxSq = m_maxSpeed * m_maxSpeed;
	if (lenSq > maxSq) 
	{
		m_velocity *= maxSq / lenSq;
	}
}

void MoveComponent2::LimitAccel()
{
	float lenSq = m_accel.LengthSq();
	float maxSq = m_maxAccel * m_maxAccel;
	if (lenSq > maxSq)
	{
		m_accel *= maxSq / lenSq;
	}
}