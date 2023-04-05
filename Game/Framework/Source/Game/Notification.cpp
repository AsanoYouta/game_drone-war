#include "Notification.h"
#include "Game.h"
#include "FPSObject.h"
#include "AudioSystem.h"

Notification::Notification(std::wstring text, float displayTime, UINT seNum, Vector2 pos)
	:UIScreen()
	,m_displayTime(displayTime)
	,m_elapsed(0.0f)
{
	auto playerObj = dynamic_cast<FPSObject*>(GAME.GetPlayer());

	m_text.reset(new Text(text, pos, D2D.g_hudFormat3));
	m_text->SetDrawRect(true);

	switch (seNum)
	{
	case 1:
		GAME.GetAudioSystem()->PlayEvent("event:/Notification1");
		break;
	case 2:
		GAME.GetAudioSystem()->PlayEvent("event:/Notification2");
		break;
	default:
		GAME.GetAudioSystem()->PlayEvent("event:/Notification1");
		break;
	}
}

void Notification::Update(float deltaTime)
{
	m_elapsed += deltaTime;
	if (m_elapsed >= m_displayTime)
	{
		Close();
	}
}

void Notification::Draw()
{
	m_text->Draw();
}
