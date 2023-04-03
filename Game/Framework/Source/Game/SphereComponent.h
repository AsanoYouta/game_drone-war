#pragma once
#include "Component.h"
#include "Source/Math/Collision.h"

class SphereComponent : public Component
{
public:
	SphereComponent(class GameObject* owner, int updateOrder = 100);
	~SphereComponent();

	void Update(float deltaTime) override;
	void OnUpdateWorldTransform() override;
	void SetObjectSphere(const Sphere& sphere) { m_worldSphere = sphere; }
	const Sphere& GetWorldSphere() const { return m_worldSphere; }
	const Sphere& GetPreWorldSphere() const { return m_preWorldSphere; }

private:
	Sphere m_objectSphere;
	Sphere m_worldSphere;
	Sphere m_preWorldSphere;
};

