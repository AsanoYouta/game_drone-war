#include "ObjectInstance.h"

ObjectInstance::ObjectInstance(GameObject* owner)
	:m_owner(owner)
	,m_color(XMFLOAT4(1.0f,1.0f,1.0f,1.0f))
	,m_isLive(true)
{
	SyncOwner();
	owner->AddInstance(this);
}

ObjectInstance::~ObjectInstance()
{
	m_owner->RemoveInstance(this);
}

void ObjectInstance::Update(float deltaTime)
{

}

void ObjectInstance::ComputeWorldTransform()
{
	m_worldMat = XMMatrixScaling(m_scale, m_scale, m_scale);
	m_worldMat *= XMMatrixRotationQuaternion(ConvertToXMVECTOR(m_rotation));
	m_worldMat *= XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
}

void ObjectInstance::SyncOwner()
{
	m_position = m_owner->GetPosition();
	m_rotation = m_owner->GetRotation();
	m_scale = m_owner->GetScale();
	m_worldMat = m_owner->GetWorldTransform();
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}
