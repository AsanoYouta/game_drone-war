#pragma once
#include <string>
#include<DirectXMath.h>
using namespace DirectX;

//イベントインスタンスのポーズやボリューム調整などを行う
class SoundEvent
{
public:
	SoundEvent();

	bool IsValid();
	void Restart();
	void Stop(bool allowFadeOut = true);

	bool Is3D() const;
	void Set3DAttributes(const XMMATRIX& worldTrans);

	void SetPaused(bool pause);
	void SetVolume(float value);
	void SetPitch(float value);
	void SetParameter(const std::string& name, float value);
	bool GetPaused() const;
	float GetVolume() const;
	float GetPitch() const;
	float GetParameter(const std::string& name);

protected:
	//AudioSystemだけがコンスタラクタを使えるように
	friend class AudioSystem;
	SoundEvent(class AudioSystem* system, unsigned int id);

private:
	class AudioSystem* m_system;
	unsigned int m_ID;
};

