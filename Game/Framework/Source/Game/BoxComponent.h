#pragma once
#include "Component.h"
#include "Source/Math/Collision.h"

class BoxComponent : public Component
{
public:
	BoxComponent(class GameObject* owner, int updateOrder = 100);
	~BoxComponent();

	void Update(float deltaTime) override;
	void OnUpdateWorldTransform() override;
	void SetObjectBox(const AABB& aabb) { m_objectBox = aabb; }
	const AABB& GetWorldBox() const { return m_worldBox; }
	const AABB& GetPreWorldBox() const { return m_preWorldBox; }
	void SetShouldRotate(bool value) { m_shouldRotate = value; }

private:
	AABB m_objectBox;
	AABB m_worldBox;
	AABB m_preWorldBox;
	bool m_shouldRotate;
};

