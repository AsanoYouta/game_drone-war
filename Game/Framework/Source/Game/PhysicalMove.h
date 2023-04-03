#pragma once
#include "Component.h"
#include "Source/Math/mymath.h"
#include "PhysWorld.h"

class PhysicalMove : public Component
{
public:
	PhysicalMove(class GameObject* owner, float mass = 1.0f, unsigned int updateOrder = 10);

	void Update(float deltaTime) override;
	void OnCollision(const CollisionInfo& ColInfo);
	//èuä‘ìIÇ»óÕÇâ¡Ç¶ÇÈ
	void AddStrikeForce(const Vector3& force) { m_strikeForce += force; };
	//nïbä‘óÕÇâ¡Ç¶ÇÈ
	void AddForce(const Vector3& force, float addForceTime) { m_addForce = force; m_addForceTime = addForceTime; }
	void LimitVelocity();

	void SetMass(float mass) { m_mass = mass; }
	void SetMaxSpeed(float max) { m_maxSpeed = max; }
	void SetUseGravity(bool use) { m_useGravity = use; }
	void SetVelocity(const Vector3& vel) { m_velocity = vel; }
	const Vector3 GetVelocity() const { return m_velocity; }
	const Vector3 GetAcceleration() const { return m_acceleration; }

private:
	Vector3 m_strikeForce;
	Vector3 m_addForce;
	Vector3 m_acceleration;
	Vector3 m_velocity;
	bool m_useGravity;
	float m_mass;
	float m_deltaTime;
	float m_maxSpeed;
	float m_addForceTime;
	float m_e; //îΩî≠åWêî
};

