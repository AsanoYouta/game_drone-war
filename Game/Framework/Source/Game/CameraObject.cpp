#include "CameraObject.h"
#include "CameraComponent.h"
#include "AudioComponent.h"

CameraObject::CameraObject()
	:GameObject()
	,m_cPitch(0.0f)
{
	m_camComp = new CameraComponent(this);
	m_audioComp = new AudioComponent(this);
}

void CameraObject::UpdateGameObject(float deltaTime)
{
	auto cameraPos = m_camComp->GetCamPos();

	Vector3 targetPos = cameraPos + GetForward() * 100.0f;

	XMMATRIX viewMat = XMMatrixLookAtLH(
		ConvertToXMVECTOR(cameraPos),
		ConvertToXMVECTOR(targetPos),
		ConvertToXMVECTOR(GetUp()));

	m_camComp->SetViewAndProjMatrix(viewMat);
}

void CameraObject::SetActive()
{
	m_camComp->SetActive();
}