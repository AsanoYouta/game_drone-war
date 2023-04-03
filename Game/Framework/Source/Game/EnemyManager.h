#pragma once
#include "GameObject.h"
#include "EnemySpawner.h"
#include "EnemyObject.h"

class EnemyManager : public GameObject
{
public:
	EnemyManager(class GameManager* gameManager, UINT waveNum);

	void UpdateGameObject(float deltaTime) override;

	void StartWave();
	void SpawnFromPos(UINT useSpawnerNum);
	void AddSpawnPos(const Vector3& pos);
	void AddEnemy(class EnemyObject* enemy);
	void RemoveEnemy(class EnemyObject* enemy);
	void AddSpawner() { m_curSpawnerNum++; }
	void RemoveSpawner() { m_curSpawnerNum--; }

	UINT GetCurrentWave() { return m_curWave; }
	UINT GetWaveNum() { return m_waveNum; }
	void SetStartEnemyNum(UINT num) { m_startEnemyNum = num; }
	void SetEnemyIncreseNum(UINT num) { m_enemyIncNum = num; }
	void SetEnemyMaxSpeed(float speed) { m_maxSpeed = speed; }
	void SetIncreseSpeed(float speed) { m_incSpeed = speed; }

private:
	class GameManager* m_gameManager;
	std::vector<Vector3> m_spawnerPositions;
	std::vector<class EnemyObject*> m_enemies;
	UINT m_curSpawnerNum;
	UINT m_waveNum;
	UINT m_curWave;
	UINT m_startEnemyNum;
	UINT m_enemyIncNum;
	float m_waveInterval;
	float m_maxSpeed;
	float m_incSpeed;
	bool m_inWave;
};

