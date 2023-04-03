#include "AudioComponent.h"
#include "GameObject.h"
#include "Game.h"

AudioComponent::AudioComponent(GameObject* owner, int updateOrder)
	:Component(owner, updateOrder)
{}

AudioComponent::~AudioComponent()
{
	StopAllEvents();
}

void AudioComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);

	auto it = m_events2D.begin();
	while (it != m_events2D.end())
	{
		if (!it->IsValid())
		{
			it = m_events2D.erase(it);
		}
		else
		{
			++it;
		}
	}

	it = m_events3D.begin();
	while (it != m_events3D.end())
	{
		if (!it->IsValid())
		{
			it = m_events3D.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void AudioComponent::OnUpdateWorldTransform()
{
	XMMATRIX world = m_owner->GetWorldTransform();
	for (auto& event : m_events3D)
	{
		if (event.IsValid())
		{
			event.Set3DAttributes(world);
		}
	}
}

SoundEvent AudioComponent::PlayEvent(const std::string& name)
{
	SoundEvent e = GAME.GetAudioSystem()->PlayEvent(name);
	if (e.Is3D())
	{
		m_events3D.emplace_back(e);
		e.Set3DAttributes(m_owner->GetWorldTransform());
	}
	else
	{
		m_events2D.emplace_back(e);
	}
	return e;
}

void AudioComponent::StopAllEvents()
{
	for (auto& e : m_events2D)
	{
		e.Stop();
	}
	for (auto& e : m_events3D)
	{
		e.Stop();
	}
	m_events2D.clear();
	m_events3D.clear();
}
