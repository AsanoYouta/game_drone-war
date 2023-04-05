#include "CameraComponent.h"
#include "GameObject.h"
#include "Component.h"
#include "Game.h"

CameraComponent::CameraComponent(GameObject* owner, int updateOrder)
	:Component(owner, updateOrder)
	,m_velocity(Vector3::Zero)
	,m_mulAmount(1.0f)
	,m_maxAmount(Vector2::Zero)
	,m_lastMove(0.0f)
	,m_moveInterval(0.0f)
{
	std::random_device rnd; //初期シード
	m_rnd.seed(rnd());
}

void CameraComponent::Update(float deltaTime)
{
	m_camPos = m_owner->GetPosition();
	
	m_lastMove += deltaTime;
	if (m_lastMove >= m_moveInterval)
	{
		m_lastMove = 0.0f;
		m_maxAmount *= m_mulAmount; //振り幅減衰
		std::uniform_real_distribution<> dx(0, m_maxAmount.x);
		std::uniform_real_distribution<> dy(0, m_maxAmount.y);
		m_velocity = Vector3(float(dx(m_rnd)), float(dy(m_rnd)), 0.0f);
		m_camPos += m_velocity * deltaTime;
	}
}

void CameraComponent::SetViewAndProjMatrix(const XMMATRIX& view)
{
	D3D.SetViewMatrix(view);

	//プロジェクション行列
	float fov = XMConvertToRadians(75.0f);
	float aspect = D3D.m_viewPort.Width / D3D.m_viewPort.Height;
	float nearZ = 0.1f;
	float farZ = 50000.0f;
	XMMATRIX projMat = XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
	D3D.SetProjMatrix(projMat);
}

void CameraComponent::SetActive()
{
	GAME.SetActiveCam(this);
}

void CameraComponent::Wiggle(float moveInt, float mulAmount, float moveAmount)
{
	m_maxAmount.x += moveAmount;
	m_maxAmount.y += moveAmount;
	m_mulAmount = mulAmount;
	m_moveInterval = moveInt;
}
void CameraComponent::Wiggle(float moveInt, float mulAmount, Vector2 moveAmount)
{
	m_maxAmount.x += moveAmount.x;
	m_maxAmount.y += moveAmount.y;
	m_mulAmount = mulAmount;
	m_moveInterval = moveInt;
}
