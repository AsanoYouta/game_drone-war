#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(class GameObject* origin, float mass = 1.0f);

	void UpdateGameObject(float deltaTime) override;
	void AddStrikeForce(const Vector3& force);
	//�Փˎ��ɌĂяo�����
	void OnCollision(const CollisionInfo& info) override;

private:
	class GameObject* m_origin; //������(�������̃R���C�_�[�Փ˂�h������)
	class PhysicalMove* m_physicalMove;
	class SphereComponent* m_sphereComp;
	float m_lifespan;
};

