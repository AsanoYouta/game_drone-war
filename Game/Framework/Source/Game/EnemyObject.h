#pragma once
#include "GameObject.h"
#include "EnemyManager.h"
#include "SoundEvent.h"

class EnemyObject : public GameObject
{
public:
	EnemyObject(class EnemyManager* manager = nullptr);
	~EnemyObject();

	void UpdateGameObject(float deltaTime) override;
	void TakeDamage(float damage, const Vector3& knockBack);
	void Destroy();

	float GetMaxSpeed() const { return m_maxSpeed; }
	class PhysicalMove* GetPhysicalMove() const { return m_physicalMove; }
	void SetMaxSpeed(float speed) { m_maxSpeed = speed; }
	void SetTargetObject(GameObject* obj) { m_targetObj = obj; }

	float g_giveDamage = 5.0f;

protected:
	class EnemyManager* m_manager;
	class PhysicalMove* m_physicalMove;
	class AudioComponent* m_audioComp;
	class MeshComponent* m_meshComp;
	class BoxComponent* m_boxComp;
	class GameObject* m_targetObj;

	//SE
	SoundEvent m_drive;

	//ÉpÉâÉÅÅ[É^
	float m_hp;
	float m_maxSpeed;
};

//SEÇçƒê∂Ç∑ÇÈplayerÇ∆ÇÃç≈í∑ãóó£
#define SOUND_DIST (400.0f)

