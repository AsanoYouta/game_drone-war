#pragma once
#include <unordered_map>
#include "framework.h"
#include "SoundEvent.h"
#include<DirectXMath.h>
using namespace DirectX;

//前方宣言
namespace FMOD
{
	class System;
	namespace Studio
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	};
};

class AudioSystem
{
public:
	AudioSystem();
	~AudioSystem();

	bool Initialize();
	void Shutdown();
	void Update(float deltaTime);

	void LoadBank(const std::string& bankName);
	void UnloadBank(const std::string& bankName);
	void UnloadAllBanks();

	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);

	SoundEvent PlayEvent(const std::string& name);

	void SetListner(const XMMATRIX& viewMat);

protected:
	//ラッピングされたイベントインスタンス
	friend class SoundEvent;
	//EventInstanceゲッター(ポインタを生で渡すのは危険)
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);

private:
	FMOD::Studio::System* m_system;
	FMOD::System* m_lowLevelSystem;

	std::unordered_map<std::string, FMOD::Studio::Bank*> m_banks;
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> m_events;
	//イベントインスタンス管理(再生した音)
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> m_eventInstances;
	static unsigned int sNextID;
	std::vector<FMOD::Studio::EventInstance*> m_pausedSounds;
	//バス(サウンドグループ)
	std::unordered_map<std::string, FMOD::Studio::Bus*> m_buses;
};

