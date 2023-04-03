#pragma once
#include "Component.h"
#include "Source/Math/Collision.h"

//�eSpeed��ݒ肵�A����ɔ�����SetPos,SetRotation���s��
class MoveComponent : public Component
{
public:
	MoveComponent(class GameObject* owner, int updateOrder = 10); //�X�V�D��x��
	void Update(float deltaTime) override;

	float GetAngularSpeed() const { return m_angularSpeed; }
	float GetForwardSpeed() const { return m_forwardSpeed; }
	float GetStrafeSpeed() const { return m_strafeSpeed; }
	float GetUpSpeed() const { return m_upSpeed; }
	void SetAngularSpeed(float speed) { m_angularSpeed = speed; }
	void SetForwardSpeed(float speed) { m_forwardSpeed = speed; }
	void SetStrafeSpeed(float speed) { m_strafeSpeed = speed; }
	void SetUpSpeed(float speed) { m_upSpeed = speed; }
	void AddUpSpeed(float speed) { m_upSpeed += speed; }
	void SetMaxSpeed(float speed) { m_maxSpeed = speed; }

private:
	float m_angularSpeed;
	float m_forwardSpeed;
	float m_strafeSpeed;
	float m_upSpeed;
	float m_maxSpeed;
};

