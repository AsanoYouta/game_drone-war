#pragma once
#include<cstdint>

class Component
{
public:
	Component(class GameObject* owner, int updateOrder = 100);
	virtual ~Component();

	virtual void Update(float deltaTime);
	virtual void OnUpdateWorldTransform(){}
	virtual void ProcessInput(const struct InputState& state){}

	int GetUpdateOrder() const { return m_updateOrder; }
	class GameObject* GetOwner() { return m_owner; }

protected:
	class GameObject* m_owner;
	//更新順序(小さいほど早く更新される)
	int m_updateOrder;
};

