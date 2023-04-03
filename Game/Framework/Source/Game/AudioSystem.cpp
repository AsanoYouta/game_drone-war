#include "AudioSystem.h"
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include "Source/Math/mymath.h"
#include "Game.h"

unsigned int AudioSystem::sNextID = 0;

AudioSystem::AudioSystem()
	:m_system(nullptr)
	,m_lowLevelSystem(nullptr)
{
}

AudioSystem::~AudioSystem()
{}

bool AudioSystem::Initialize()
{
	FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_ERROR, FMOD_DEBUG_MODE_TTY);

	//システムオブジェクト生成
	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&m_system);
	if (result != FMOD_OK)
	{
		DEBUG_LOG("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	//FMODStudioSystem初期化
	result = m_system->initialize(
		512,					//最大同時発音数
		FMOD_STUDIO_INIT_NORMAL,
		FMOD_INIT_NORMAL,
		nullptr
	);
	if (result != FMOD_OK)
	{
		DEBUG_LOG("Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	//低レベルシステムポインタ取得
	m_system->getLowLevelSystem(&m_lowLevelSystem);

	//マスターバンクをロード(stringsが先)
	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

void AudioSystem::Shutdown()
{
	//すべてのバンクを開放
	if (m_system)
	{
		m_system->release();
	}
}

void AudioSystem::Update(float deltaTime)
{
	std::vector<unsigned int> done;
	for (auto it : m_eventInstances)
	{
		FMOD::Studio::EventInstance* e = it.second;
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			e->release();
			done.emplace_back(it.first);
		}
	}
	for (auto id : done)
	{
		m_eventInstances.erase(id);
	}

	//Pauseの場合、ストリーム音源以外ポーズにする
	if (GAME.GetState() != Game::GamePlay)
	{
		for (auto it : m_eventInstances)
		{
			FMOD::Studio::EventDescription* desc;
			it.second->getDescription(&desc);
			bool isStream;
			desc->isStream(&isStream);
			if (!isStream)
			{
				it.second->setPaused(true);
				m_pausedSounds.emplace_back(it.second);
			}
		}
	}
	else
	{
		if (!m_pausedSounds.empty())
		{
			for (auto event : m_pausedSounds)
			{
				event->setPaused(false);
			}
			m_pausedSounds.clear();
		}
	}

	m_system->update();
}

void AudioSystem::LoadBank(const std::string& bankName)
{
	//二重ロード回避
	if (m_banks.find(bankName) != m_banks.end()) return;

	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = 
		m_system->loadBankFile( //バンクをロード
		bankName.c_str(),
		FMOD_STUDIO_LOAD_BANK_NORMAL,
		&bank
	);

	const int maxPathLength = 512;
	if (result == FMOD_OK)
	{
		m_banks.emplace(bankName, bank);
		//サンプルデータをロード
		bank->loadSampleData();
		//このバンクにあるイベントの数を取得
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0)
		{
			//各イベント記述子を取得
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* e = events[i];
				//イベントのパス取得
				e->getPath(eventName, maxPathLength, nullptr);
				//各バンク間でイベントは一つのコンテナに格納する
				m_events.emplace(eventName, e);
				DEBUG_LOG("EventName: %s\n", eventName);
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& bankName)
{
	auto iter = m_banks.find(bankName);
	if (iter == m_banks.end()) return;

	//バンク内のイベント削除
	FMOD::Studio::Bank* bank = iter->second;
	int num_events = 0;
	bank->getEventCount(&num_events);
	if (num_events > 0)
	{
		std::vector<FMOD::Studio::EventDescription*> events(num_events);
		bank->getEventList(events.data(), num_events, &num_events);
		char eventName[512];
		for (int i = 0; i < num_events; i++)
		{
			FMOD::Studio::EventDescription* e = events[i];
			e->getPath(eventName, 512, nullptr);
			auto eventi = m_events.find(eventName);
			if (eventi != m_events.end())
			{
				m_events.erase(eventi);
			}
		}
	}

	int num_buses = 0;
	bank->getBusCount(&num_buses);
	if (num_buses > 0)
	{
		std::vector<FMOD::Studio::Bus*> buses(num_buses);
		bank->getBusList(buses.data(), num_buses, &num_buses);
		char busName[512];
		for (int i = 0; i < num_buses; i++)
		{
			FMOD::Studio::Bus* bus = buses[i];
			bus->getPath(busName, 512, nullptr);
			auto busi = m_buses.find(busName);
			if (busi != m_buses.end())
			{
				m_buses.erase(busi);
			}
		}
	}

	bank->unloadSampleData();
	bank->unload();
	m_banks.erase(iter);
}

//全てのバンク、イベントを削除
void AudioSystem::UnloadAllBanks()
{
	for (auto& iter : m_banks)
	{
		iter.second->unloadSampleData();
		iter.second->unload();
	}
	m_banks.clear();
	m_events.clear();
}

void AudioSystem::SetListner(const XMMATRIX& viewMat)
{
	XMFLOAT4X4 invView; 
	XMStoreFloat4x4(&invView, XMMatrixInverse(nullptr, viewMat));
	FMOD_3D_ATTRIBUTES listner;
	listner.position = { invView._41, invView._42, invView._43 };
	auto forward = Vector3::Normalized(Vector3(invView._31, invView._32, invView._33));
	listner.forward = { forward.x, forward.y, forward.z };
	auto up = Vector3::Normalized(Vector3(invView._21, invView._22, invView._23));
	listner.up = { up.x, up.y, up.z };
	listner.velocity = { 0.0f, 0.0f, 0.0f };

	m_system->setListenerAttributes(0, &listner);
}

SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
	unsigned int retID = 0;
	//イベントの存在を確認
	auto it = m_events.find(name);
	if (it != m_events.end())
	{
		FMOD::Studio::EventInstance* event = nullptr;
		it->second->createInstance(&event);
		if (event)
		{
			event->start();
			//DEBUG_LOG("PlayEvent : %s\n", name.c_str());
			//次のIDを取得して連想配列に追加
			sNextID++;
			retID = sNextID;
			//DEBUG_LOG("sNextID : %i\n", retID);
			m_eventInstances.emplace(retID, event);
		}
	}
	return SoundEvent(this, retID);
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
{
	FMOD::Studio::EventInstance* event = nullptr;
	auto it = m_eventInstances.find(id);
	if (it != m_eventInstances.end())
	{
		event = it->second;
	}
	return event;
}

float AudioSystem::GetBusVolume(const std::string& name) const
{
	float retVal = 0.0f;
	const auto iter = m_buses.find(name);
	if (iter != m_buses.end())
	{
		iter->second->getVolume(&retVal);
	}
	return retVal;
}

bool AudioSystem::GetBusPaused(const std::string& name) const
{
	bool retVal = false;
	const auto iter = m_buses.find(name);
	if (iter != m_buses.end())
	{
		iter->second->getPaused(&retVal);
	}
	return retVal;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume)
{
	auto iter = m_buses.find(name);
	if (iter != m_buses.end())
	{
		iter->second->setVolume(volume);
	}
}

void AudioSystem::SetBusPaused(const std::string& name, bool pause)
{
	auto iter = m_buses.find(name);
	if (iter != m_buses.end())
	{
		iter->second->setPaused(pause);
	}
}
