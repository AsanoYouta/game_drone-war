#include "BoxComponent.h"
#include "PhysWorld.h"
#include "GameObject.h"
#include "Game.h"

BoxComponent::BoxComponent(GameObject* owner, int updateOrder)
	:Component(owner, updateOrder)
	,m_objectBox(Vector3::Zero, Vector3::Zero)
	,m_worldBox(Vector3::Zero, Vector3::Zero)
	,m_preWorldBox(Vector3::Zero, Vector3::Zero)
	,m_shouldRotate(true)
{
	GAME.GetPhysWorld()->AddBoxComp(this);
}

BoxComponent::~BoxComponent()
{
	GAME.GetPhysWorld()->RemoveBoxComp(this);
}

void BoxComponent::Update(float deltaTime)
{

}

void BoxComponent::OnUpdateWorldTransform()
{
	m_preWorldBox = m_worldBox;
	//Mesh内で初期化したオブジェクト空間のボックスへリセット
	m_worldBox = m_objectBox;

	m_worldBox.m_max *= m_owner->GetScale();
	m_worldBox.m_min *= m_owner->GetScale();

	if(m_shouldRotate)
		m_worldBox.Rotate(m_owner->GetRotation());

	m_worldBox.m_min += m_owner->GetPosition();
	m_worldBox.m_max += m_owner->GetPosition();
}
