#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(class GameObject* origin, float mass = 1.0f);

	void UpdateGameObject(float deltaTime) override;
	void AddStrikeForce(const Vector3& force);
	//衝突時に呼び出される
	void OnCollision(const CollisionInfo& info) override;

private:
	class GameObject* m_origin; //生成元(生成元のコライダー衝突を防ぐため)
	class PhysicalMove* m_physicalMove;
	class SphereComponent* m_sphereComp;
	float m_lifespan;
};

