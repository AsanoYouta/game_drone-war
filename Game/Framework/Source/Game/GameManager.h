#pragma once
#include "GameObject.h"
#include "SoundEvent.h"

class GameManager : public GameObject
{
public:
	GameManager();

	void UpdateGameObject(float deltaTime) override;
	void StartGame();
	void ClearGame();

	bool GetInGame() const { return m_inGame; }
	float GetGameTime() const { return m_gameTime; }
	class EnemyManager* GetEnemyManager() const { return m_enemyManager; }
	void SetInGame(bool inGame) { m_inGame = inGame; }
	void SetEnemyManager(class EnemyManager* manager) { m_enemyManager = manager; }


private:
	bool m_inGame;
	bool m_cleard;
	float m_gameTime;
	float m_elapsed;
	class EnemyManager* m_enemyManager;

	class AudioComponent* m_audioComp;
	SoundEvent m_music;
};

