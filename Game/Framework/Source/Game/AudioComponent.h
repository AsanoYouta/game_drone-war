#pragma once
#include "Component.h"
#include "framework.h"
#include "SoundEvent.h"
#include "AudioSystem.h"

class AudioComponent : public Component
{
public:
	AudioComponent(class GameObject* owner, int updateOrder = 200);
	~AudioComponent();

	void Update(float deltaTime) override;
	void OnUpdateWorldTransform() override;

	SoundEvent PlayEvent(const std::string& name);
	void StopAllEvents();

private:
	std::vector<SoundEvent> m_events2D;
	std::vector<SoundEvent> m_events3D;
};

