#pragma once
#include "GameObject.h"

class BallObject : public GameObject
{
public:
	BallObject();

	void UpdateGameObject(float deltaTime) override;
	void AddStrikeForce(const Vector3& force);
private:
	class PhysicalMove* m_physicalMove;
};

