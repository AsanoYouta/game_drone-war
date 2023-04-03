#pragma once
#include "Component.h"
#include "Source/DirectX/DirectX.h"
#include <random>

class CameraComponent : public Component
{
public:
	CameraComponent(class GameObject* owner, int updateOrder = 200);

	void Update(float deltaTime) override;

	//カメラを小刻みに揺らす
	void Wiggle(
		float interval,  //一度揺らすまでのインターバル
		float mulAmount, //振れ幅に毎回かける数値
		float amount);   //振れ幅
	void Wiggle(float interval, float mulAmount, Vector2 amount);

	//Game側でアクティブカメラをセットする
	void SetActive();
	void SetViewAndProjMatrix(const XMMATRIX& view);
	const Vector3& GetCamPos() const { return m_camPos; }

private:
	Vector3 m_camPos;
	//カメラの揺れ
	Vector3 m_velocity;
	Vector2 m_maxAmount;
	float m_mulAmount;
	float m_moveInterval;
	float m_lastMove;
	//疑似乱数生成器
	std::mt19937 m_rnd;
};

