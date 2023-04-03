#include "GameManager.h"
#include "EnemyManager.h"
#include "Game.h"
#include "FPSObject.h"
#include "ClearStage.h"
#include "AudioComponent.h"

GameManager::GameManager()
	:GameObject()
	,m_inGame(false)
	,m_enemyManager(nullptr)
	,m_gameTime(0.0f)
	,m_elapsed(0.0f)
	,m_cleard(false)
{
	auto player = GAME.GetPlayer();
	auto fpsObj = dynamic_cast<FPSObject*>(player);
	if (fpsObj)
	{
		fpsObj->SetGameManager(this);
	}
	else
	{
		DEBUG_LOG("GameManager:Not Exist PlayerObject\n");
	}

	m_audioComp = new AudioComponent(this);
}

void GameManager::UpdateGameObject(float deltaTime)
{
	if (m_inGame)
	{
		m_gameTime += deltaTime;
	}

	if (m_cleard)
	{
		m_elapsed += deltaTime;
		if (m_elapsed >= 3.0f)
		{
			new ClearStage(m_gameTime);
		}
	}
}

void GameManager::StartGame()
{
	if (!m_enemyManager)
	{
		DEBUG_LOG("GameManager:Not assigned EnemyManager\n");
		return;
	}
	m_inGame = true;
	m_enemyManager->StartWave();
	
	m_music = m_audioComp->PlayEvent("event:/Music_Level");
	m_music.SetVolume(0.6f);
	DEBUG_LOG("StartGame\n");
}

void GameManager::ClearGame()
{
	m_inGame = false;
	m_cleard = true;
}
