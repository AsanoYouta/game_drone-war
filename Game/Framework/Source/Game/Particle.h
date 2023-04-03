#pragma once
#include "ObjectInstance.h"

class Particle : public ObjectInstance
{
public:
	Particle(GameObject* owner, 
		Vector3& velocity, 
		float lifespan, 
		float mulVec,
		float addScale,
		XMFLOAT4 addCol);

	void Update(float deltaTime) override;

	void SetGravityAccel(float gAccel) { m_gAccel = gAccel; }

private:
	Vector3 m_velocity;
	float m_lifespan;
	float m_mulVec;
	float m_addScale;
	float m_gAccel;
	XMFLOAT4 m_addCol;
};

