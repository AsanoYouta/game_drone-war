#pragma once
#include "GameObject.h"
#include "EnemyManager.h"

class EnemySpawner : public GameObject
{
public:
	EnemySpawner(class EnemyManager* manager, UINT spawnNum, float timeToOpen);
	~EnemySpawner();

	void UpdateGameObject(float deltaTime) override;
	void Open();
	void SetEnemyMaxSpeed(float speed) { m_enemyMaxSpeed = speed; }
	
private:
	class EnemyManager* m_manager;
	class PhysicalMove* m_physicalMove;
	class MeshComponent* m_meshComp;
	float m_timeToOpen;
	float m_enemyMaxSpeed;
	UINT m_spawnNum;
};

