#include "SoundEvent.h"
#include "AudioSystem.h"
#include <fmod_studio.hpp>
#include "Source/Math/mymath.h"

SoundEvent::SoundEvent(class AudioSystem* system, unsigned int id)
	:m_system(system)
	,m_ID(id)
{
}

SoundEvent::SoundEvent()
	:m_system(nullptr)
	,m_ID(0)
{}

bool SoundEvent::IsValid()
{
	return (m_system && m_system->GetEventInstance(m_ID) != nullptr);
}

void SoundEvent::Restart()
{
	auto event = m_system ? m_system->GetEventInstance(m_ID) : nullptr; //m_systemのnullチェック
	if (event) //eventのnullチェック
	{
		event->start();
	}
}

void SoundEvent::Stop(bool fadeOut)
{
	auto event = m_system ? m_system->GetEventInstance(m_ID) : nullptr;
	if (event)
	{
		FMOD_STUDIO_STOP_MODE mode = fadeOut ?
			FMOD_STUDIO_STOP_ALLOWFADEOUT :
			FMOD_STUDIO_STOP_IMMEDIATE;
		event->stop(mode);
	}
}

void SoundEvent::SetPaused(bool pause)
{
	auto event = m_system ? m_system->GetEventInstance(m_ID) : nullptr;
	if (event)
	{
		event->setPaused(pause);
	}
}

void SoundEvent::SetVolume(float value)
{
	auto event = m_system ? m_system->GetEventInstance(m_ID) : nullptr;
	if (event)
	{
		event->setVolume(value);
	}
}

void SoundEvent::SetPitch(float value)
{
	auto event = m_system ? m_system->GetEventInstance(m_ID) : nullptr;
	if (event)
	{
		event->setPitch(value);
	}
}

void SoundEvent::SetParameter(const std::string& name, float value)
{
	auto event = m_system ? m_system->GetEventInstance(m_ID) : nullptr;
	if (event)
	{
		event->setParameterValue(name.c_str(), value);
	}
}

bool SoundEvent::GetPaused() const
{
	bool retVal = false;
	auto event = m_system ? m_system->GetEventInstance(m_ID) : nullptr;
	if (event)
	{
		event->getPaused(&retVal);
	}
	return retVal;
}

float SoundEvent::GetVolume() const
{
	float retVal = 0.0f;
	auto event = m_system ? m_system->GetEventInstance(m_ID) : nullptr;
	if (event)
	{
		event->getVolume(&retVal);
	}
	return retVal;
}

float SoundEvent::GetPitch() const
{
	float retVal = 0.0f;
	auto event = m_system ? m_system->GetEventInstance(m_ID) : nullptr;
	if (event)
	{
		event->getPitch(&retVal);
	}
	return retVal;
}

float SoundEvent::GetParameter(const std::string& name)
{
	float retVal = 0.0f;
	auto event = m_system ? m_system->GetEventInstance(m_ID) : nullptr;
	if (event)
	{
		event->getParameterValue(name.c_str(), &retVal);
	}
	return retVal;
}

bool SoundEvent::Is3D() const
{
	bool retVal = false;
	auto event = m_system ? m_system->GetEventInstance(m_ID) : nullptr;
	if (event)
	{
		FMOD::Studio::EventDescription* eDesc = nullptr;
		event->getDescription(&eDesc);
		if (eDesc)
		{
			eDesc->is3D(&retVal);
		}
	}
	return retVal;
}

void SoundEvent::Set3DAttributes(const XMMATRIX& worldTrans)
{
	auto event = m_system ? m_system->GetEventInstance(m_ID) : nullptr;
	if (event)
	{
		FMOD_3D_ATTRIBUTES attri;
		XMFLOAT4X4 mat;
		XMStoreFloat4x4(&mat, worldTrans);
		attri.position = { mat._41, mat._42, mat._43 };
		auto forward   = Vector3::Normalized(Vector3(mat._31, mat._32, mat._33));
		attri.forward  = { forward.x, forward.y, forward.z };
		auto up		   = Vector3::Normalized(Vector3(mat._21, mat._22, mat._33));
		attri.up       = { up.x, up.y, up.z };

		event->set3DAttributes(&attri);
	}
}