#pragma once
#include "Component.h"
#include "Source/DirectX/DirectX.h"
#include <random>

class CameraComponent : public Component
{
public:
	CameraComponent(class GameObject* owner, int updateOrder = 200);

	void Update(float deltaTime) override;

	//�J�����������݂ɗh�炷
	void Wiggle(
		float interval,  //��x�h�炷�܂ł̃C���^�[�o��
		float mulAmount, //�U�ꕝ�ɖ��񂩂��鐔�l
		float amount);   //�U�ꕝ
	void Wiggle(float interval, float mulAmount, Vector2 amount);

	//Game���ŃA�N�e�B�u�J�������Z�b�g����
	void SetActive();
	void SetViewAndProjMatrix(const XMMATRIX& view);
	const Vector3& GetCamPos() const { return m_camPos; }

private:
	Vector3 m_camPos;
	//�J�����̗h��
	Vector3 m_velocity;
	Vector2 m_maxAmount;
	float m_mulAmount;
	float m_moveInterval;
	float m_lastMove;
	//�^������������
	std::mt19937 m_rnd;
};

