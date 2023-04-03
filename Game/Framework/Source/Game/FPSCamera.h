#pragma once
#include "CameraComponent.h"

class FPSCamera : public CameraComponent
{
public:
	FPSCamera(class GameObject* owner);

	void Update(float deltaTime) override;

	float GetCurPitch() const { return m_cPitch; }
	float GetMaxPitch() const { return m_maxPitch; }
	const Vector3& GetForward() const { return m_forward; }
	void SetPitchSpeed(const float speed) { m_pitchSpeed = speed; }

private:
	//単位：ラジアン
	float m_pitchSpeed;
	float m_maxPitch;
	float m_cPitch;
	Vector3 m_forward;
};

