#pragma once
#include "GameObject.h"
#include "SoundEvent.h"
#include <random>

class FPSObject : public GameObject
{
public:
	enum CharacterState
	{
		OnGround,
		InAir
	};

	FPSObject();

	void UpdateGameObject(float deltaTime) override;
	void ObjectInput(const struct InputState& state) override;

	void Shoot();
	void Dead();
	void FixAllBoxes();
	void SetPloxAlert(GameObject* obj);
	void HitEvent();
	bool ContactEnemy(GameObject* obj);

	class GameManager* GetGameManager() const { return m_gameManager; }
	class MoveComponent2* GetMoveComp() const { return m_moveComp; }
	class FPSCamera* GetFPSCam() const { return m_fpsCam; }
	UINT GetRemainBullets() const { return m_remainAmmo; }
	class EnemyObject* GetDamagedEnemy() const { return m_damagedEnemy; }
	const std::vector<class EnemyObject*>& GetCloseEnemies() const { return m_closeEnemies; }
	float GetInvincibleTime() const { return m_invincibleTime; }
	float GetHp() const { return m_hp; }
	float GetJumpCharge() const { return m_jumpCharge; }
	float GetStepCharge() const { return m_stepCharge; }
	float GetOneStepSec() const { return m_oneStepSec; }
	float GetHitMarkerDisplay() const { return m_hitMarkerDisplay; }
	void SetGameManager(class GameManager* manager) { m_gameManager = manager; }

private:
	class MoveComponent2* m_moveComp;
	class BoxComponent* m_boxComp;
	class FPSCamera* m_fpsCam;
	class MeshComponent* m_meshComp;
	class GameObject* m_gunModel;
	class AudioComponent* m_audioComp;
	class GameManager* m_gameManager;
	//SE
	SoundEvent m_footStep;
	SoundEvent m_environment;
	SoundEvent m_vulcan;
	SoundEvent m_charge;
	SoundEvent m_dying;
	SoundEvent m_plox;
	float m_lastFootStep;
	void PlayFootStep();
	//プレイヤーパラメータ
	CharacterState m_charaState;
	std::vector<class EnemyObject*> m_closeEnemies;
	class EnemyObject* m_damagedEnemy;
	float m_hp;
	float m_invincibleTime;
	float m_accelAmount;
	float m_jumpCharge;
	float m_stepCharge;
	float m_oneStepSec;
	float m_alertDist;
	float m_hitMarkerDisplay;
	//武装パラメータ
	bool m_shoot;
	float m_lastShot;
	float m_shootingRate;
	float m_maxSpread;
	UINT m_remainAmmo;
	//疑似乱数生成器
	std::mt19937 m_rnd;
};

