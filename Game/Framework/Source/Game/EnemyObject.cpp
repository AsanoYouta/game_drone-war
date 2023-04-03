#include "EnemyObject.h"
#include "MoveComponent.h"
#include "AudioComponent.h"
#include "AudioSystem.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"
#include "Game.h"
#include "PhysicalMove.h"
#include "Explosion.h"
#include "Smoke.h"

EnemyObject::EnemyObject(EnemyManager* manager)
	:GameObject()
	,m_manager(manager)
	,m_hp(100.0f)
	,m_maxSpeed(200.0f)
	,m_targetObj(GAME.GetPlayer())
{
	if (m_manager)
	{
		m_manager->AddEnemy(this);
	}

	m_physicalMove = new PhysicalMove(this, 0.6f);
	m_physicalMove->SetUseGravity(false);
	m_physicalMove->SetMaxSpeed(m_maxSpeed);

	m_audioComp = new AudioComponent(this);

	m_meshComp = new MeshComponent(this);
	auto mesh = D3D.GetMesh("Assets/Models/Enemy_Drone.gpmesh");
	m_meshComp->SetMesh(mesh);
	auto mat = material::BrightWhite;
	mat.ambient = XMFLOAT4(2.0f, 2.0f, 2.0f, 0.0f);
	mat.emissive = XMFLOAT4(0.15f, 0.1f, 0.1f, 0.0f);
	m_meshComp->SetMaterial(mat);

	m_boxComp = new BoxComponent(this);
	m_boxComp->SetObjectBox(mesh->GetAABB());

	SetScale(10.0f);
}

EnemyObject::~EnemyObject()
{
	if(m_manager)
		m_manager->RemoveEnemy(this);
}

void EnemyObject::UpdateGameObject(float deltaTime)
{
	m_physicalMove->SetMaxSpeed(m_maxSpeed);
	auto velocity = m_physicalMove->GetVelocity();
	RotateToNewForward(velocity.Normalized());

	if (!m_targetObj) return;
	//SE§Œä
	float dist = Vector3::DistanceSq(this->GetPosition(), m_targetObj->GetPosition());
	if (dist <= SOUND_DIST * SOUND_DIST)
	{
		if(!m_drive.IsValid()) 
			m_drive = m_audioComp->PlayEvent("event:/Drone_Drive");
	}
	else
	{
		m_drive.Stop();
	}
}

void EnemyObject::TakeDamage(float damage, const Vector3& knockBack)
{
	m_hp -= damage;
	m_physicalMove->AddForce(knockBack, 1.0f);
	if (m_hp <= 0)
	{
		Destroy();
	}
}

void EnemyObject::Destroy()
{
	//new Smoke(GetPosition(), 0.2f);
	new Explosion(GetPosition());
	SetState(Dead);
}
