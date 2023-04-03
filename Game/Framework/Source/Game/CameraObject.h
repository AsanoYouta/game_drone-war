#pragma once
#include "GameObject.h"

class CameraObject : public GameObject
{
public:
	CameraObject();

	void UpdateGameObject(float deltaTime) override;

	void SetActive();
	void SetPitch(float pitch) { m_cPitch = pitch; }
	class CameraComponent* GetCameraComp() { return m_camComp; }
	class AudioComponent* GetAudioComp() { return m_audioComp; }

private:
	class CameraComponent* m_camComp;
	class AudioComponent* m_audioComp;

	float m_cPitch;
};

