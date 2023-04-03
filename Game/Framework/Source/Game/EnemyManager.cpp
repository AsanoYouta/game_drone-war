#include "EnemyManager.h"
#include "GameManager.h"
#include "Notification.h"

EnemyManager::EnemyManager(GameManager* gameManager, UINT waveNum)
	:GameObject()
	,m_gameManager(gameManager)
	,m_waveNum(waveNum)
	,m_curWave(0)
	,m_startEnemyNum(30)
	,m_enemyIncNum(30)
	,m_maxSpeed(200.0f)
	,m_incSpeed(40.0f)
	,m_waveInterval(5.0f)
	,m_curSpawnerNum(0)
	,m_inWave(false)
{
	m_gameManager->SetEnemyManager(this);
}

void EnemyManager::AddSpawnPos(const Vector3& pos)
{
	m_spawnerPositions.emplace_back(pos);
}

void EnemyManager::AddEnemy(EnemyObject* enemy)
{
	m_enemies.emplace_back(enemy);
}

void EnemyManager::RemoveEnemy(EnemyObject* enemy)
{
	auto it = std::find(m_enemies.begin(), m_enemies.end(), enemy);
	if (it != m_enemies.end())
	{
		m_enemies.erase(it);
	}
}

void EnemyManager::UpdateGameObject(float deltaTime)
{
	m_waveInterval -= deltaTime;

	if (m_waveInterval <= 0.0f && !m_inWave)
	{
		StartWave();
	}

	if (m_gameManager->GetInGame())
	{
		//ウェーブクリア
		if (m_inWave && m_enemies.empty() && m_curSpawnerNum == 0)
		{
			m_inWave = false;
			m_gameManager->SetInGame(false); //タイム計測中断
			m_waveInterval = 5.0f;
			m_maxSpeed += m_incSpeed;

			if (m_curWave == m_waveNum)
			{
				//SetState(Dead);
				m_gameManager->ClearGame();
				DEBUG_LOG("ClearGame\n");
			}

			new Notification(L"WAVE CLEAR", 2.5f);
			DEBUG_LOG("ClearWave\n");
		}
	}

}

void EnemyManager::StartWave()
{
	m_curWave++;
	SpawnFromPos(m_curWave);
	m_inWave = true;
	m_gameManager->SetInGame(true);

	new Notification(L"WAVE"+std::to_wstring(m_curWave), 2.5f);

	DEBUG_LOG("StartWave:%d\n", m_curWave);
}

void EnemyManager::SpawnFromPos(UINT useSpawnerNum)
{
	if (useSpawnerNum > m_spawnerPositions.size()) 
		useSpawnerNum = m_spawnerPositions.size();

	UINT spawnNum = m_startEnemyNum + m_enemyIncNum * (m_curWave-1);
	for (UINT i = 0; i < useSpawnerNum; i++)
	{
		auto spawner = new EnemySpawner(this, spawnNum/useSpawnerNum, 3.0f);
		if (m_spawnerPositions[i].IsZero())
		{
			DEBUG_LOG("EnemyManager:Not Assigned EnemySpawnPos\n");
		}
		spawner->SetPosition(m_spawnerPositions[i]);
		spawner->SetEnemyMaxSpeed(m_maxSpeed);
	}
}
