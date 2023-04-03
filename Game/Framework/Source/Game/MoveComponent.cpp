#include "MoveComponent.h"
#include "GameObject.h"
#include "Source/Math/mymath.h"

MoveComponent::MoveComponent(class GameObject* owner, int updateOrder)
	:Component(owner, updateOrder)
	, m_angularSpeed(0.0f)
	, m_forwardSpeed(0.0f)
	, m_strafeSpeed(0.0f)
	, m_upSpeed(0.0f)
	, m_maxSpeed(300.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
	//y軸回転
	if (!math::NearZero(m_angularSpeed))
	{
		Quaternion rot = m_owner->GetRotation();
		float angle = m_angularSpeed * deltaTime;
		Quaternion inc(Vector3::Up, angle);
		//DEBUG_LOG("q inc %f\n", inc.w);
		rot = Quaternion::Concatenate(rot, inc);
		m_owner->SetRotation(rot);
	}

	if (!math::NearZero(m_forwardSpeed) || 
		!math::NearZero(m_strafeSpeed)  || 
		!math::NearZero(m_upSpeed))
	{
		//スピード制限
		if (m_forwardSpeed > m_maxSpeed) m_forwardSpeed = m_maxSpeed;
		else if (m_forwardSpeed < -m_maxSpeed) m_forwardSpeed = -m_maxSpeed;
		if (m_strafeSpeed > m_maxSpeed) m_strafeSpeed = m_maxSpeed;
		else if (m_strafeSpeed < -m_maxSpeed) m_strafeSpeed = -m_maxSpeed;
		//位置更新
		Vector3 pos = m_owner->GetPosition();
		pos += m_owner->GetForward() * m_forwardSpeed * deltaTime;
		pos += m_owner->GetRight() * m_strafeSpeed * deltaTime;
		pos += Vector3::Up * m_upSpeed * deltaTime;
		m_owner->SetPosition(pos);
	}
}