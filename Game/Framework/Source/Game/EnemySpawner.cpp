#include "EnemySpawner.h"
#include "PhysicalMove.h"
#include "MeshComponent.h"
#include "SwarmEnemy.h"
#include "Spark.h"

EnemySpawner::EnemySpawner(EnemyManager* manager, UINT spawnNum, float timeToOpen)
	:GameObject()
	,m_manager(manager)
	,m_spawnNum(spawnNum)
	,m_timeToOpen(timeToOpen)
	,m_enemyMaxSpeed(200.0f)
{
	manager->AddSpawner();

	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(D3D.GetMesh("Assets/Models/Container.gpmesh"));
	SetScale(1.4f);
	m_meshComp->SetMaterial(material::BrightWhite);

	m_physicalMove = new PhysicalMove(this, 100.0f);
	m_physicalMove->SetUseGravity(true);
	m_physicalMove->AddForce(-Vector3::Up * 9500.0f, 0.1f);
}

EnemySpawner::~EnemySpawner()
{
	m_manager->RemoveSpawner();
}

void EnemySpawner::UpdateGameObject(float deltaTime)
{
	m_timeToOpen -= deltaTime;

	if (m_timeToOpen <= 0)
	{
		Open();
	}
}

void EnemySpawner::Open()
{
	for (UINT i = 0; i < m_spawnNum; i++)
	{
		auto enemy = new SwarmEnemy(m_manager);
		enemy->SetMaxSpeed(m_enemyMaxSpeed);
		enemy->SetPosition(this->GetPosition());
	}

	new Spark(this->GetPosition(), 2.0f, 8, 550.0f, 1.0f, 0.95f, -2.0f, 
		XMFLOAT4(-0.1f, 0.5f, 0.5f, 0.0f));
	SetState(Dead);
}
