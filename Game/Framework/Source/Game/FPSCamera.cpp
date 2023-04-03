#include "FPSCamera.h"
#include "GameObject.h"
#include "Source/Math/mymath.h"

FPSCamera::FPSCamera(GameObject* owner)
	:CameraComponent(owner)
	,m_pitchSpeed(0.0f)
	,m_maxPitch(PI / 3.0f) //60�x
	,m_cPitch(0.0f)
{
}

void FPSCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	Vector3 cameraPos = GetCamPos(); //���N���X�̈ʒu�擾

	m_cPitch += m_pitchSpeed * deltaTime;
	m_cPitch = std::clamp(m_cPitch, -m_maxPitch, m_maxPitch);
	auto q = Quaternion(m_owner->GetRight(), m_cPitch);
	m_forward = Vector3::Rotate(m_owner->GetForward(), q); //���[�J���̃x�N�g���͕ύX���Ȃ�
	Vector3 targetPos = cameraPos + m_forward * 100.0f;
	Vector3 up = Vector3::Rotate(Vector3::Up, q);

	XMMATRIX viewMat = XMMatrixLookAtLH(
		ConvertToXMVECTOR(cameraPos), 
		ConvertToXMVECTOR(targetPos), 
		ConvertToXMVECTOR(up));
	SetViewAndProjMatrix(viewMat);
}

