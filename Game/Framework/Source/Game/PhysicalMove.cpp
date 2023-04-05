#include "PhysicalMove.h"
#include "GameObject.h"
#include "PhysWorld.h"
#include "Game.h"

PhysicalMove::PhysicalMove(GameObject* owner, float mass, unsigned int updateOrder)
	:Component(owner, updateOrder)
	, m_strikeForce(Vector3::Zero)
	, m_addForce(Vector3::Zero)
	, m_acceleration(Vector3::Zero)
	, m_velocity(Vector3::Zero)
	, m_maxSpeed(math::InfinityF)
	, m_mass(mass)
	, m_e(0.5f)
	, m_useGravity(true)
	, m_deltaTime(0.0f)
	, m_addForceTime(0.0f)
{
}

void PhysicalMove::Update(float deltaTime)
{
	//合力計算
	m_deltaTime = deltaTime;
	m_addForceTime -= deltaTime;
	Vector3 g = Vector3::Zero;
	if(m_useGravity) g = m_mass * -Vector3::Up * 98.0f; //重力
	Vector3 sumOfForces = m_strikeForce + m_addForce + g;

	//移動量計算
	m_acceleration = Vector3::Zero;
	if (!sumOfForces.IsZero()) {
		m_acceleration = sumOfForces / m_mass;
	}
	m_velocity += m_acceleration * deltaTime;
	LimitVelocity();
	Vector3 newPos = m_owner->GetPosition() + m_velocity * deltaTime;
	m_owner->SetPosition(newPos);

	//撃力初期化/継続的に加える力を更新
	m_strikeForce = Vector3::Zero;
	if (m_addForceTime <= 0.0f)
	{
		m_addForce = Vector3::Zero;
	}
}

void PhysicalMove::OnCollision(const CollisionInfo& colInfo)
{
	Vector3 dir = Vector3::Normalized(m_velocity);
	m_owner->SetPosition(colInfo.collidePoint);
	//衝突時かかる力を計算
	float amountOfForce = (m_mass * m_velocity.Length())*(m_e + 1) / (m_deltaTime);
	m_strikeForce += -colInfo.normal * amountOfForce * Vector3::Dot(dir, colInfo.normal);
}

void PhysicalMove::LimitVelocity()
{
	if (m_maxSpeed == math::InfinityF) return;

	float lenSq = m_velocity.LengthSq();
	float maxSq = m_maxSpeed * m_maxSpeed;
	if (lenSq > maxSq)
	{
		m_velocity *= maxSq / lenSq;
	}
}
