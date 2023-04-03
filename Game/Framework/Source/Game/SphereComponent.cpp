#include "SphereComponent.h"
#include "GameObject.h"
#include "Source/Math/mymath.h"
#include "PhysWorld.h"
#include "Game.h"

SphereComponent::SphereComponent(GameObject* owner, int updateOrder)
	:Component(owner, updateOrder)
	, m_objectSphere(Vector3::Zero, 0)
	, m_worldSphere(Vector3::Zero, 0)
	, m_preWorldSphere(Vector3::Zero, 0)
{
	GAME.GetPhysWorld()->AddSphereComp(this);
}

SphereComponent::~SphereComponent()
{
	GAME.GetPhysWorld()->RemoveSphereComp(this);
}

void SphereComponent::Update(float deltaTime)
{
	//ƒV[ƒ““à‚ÌAABB‚ÆÕ“Ë‚µ‚½‚çOnCollision‚ðŒÄ‚Ño‚·
	CollisionInfo info;
	if (GAME.GetPhysWorld()->SweptAllBoxes(this, info))
	{
		m_owner->OnCollision(info);
	}
}

void SphereComponent::OnUpdateWorldTransform()
{
	m_preWorldSphere = m_worldSphere;
	m_worldSphere.m_position = m_owner->GetPosition();
}
