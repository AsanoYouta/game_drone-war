#include "ClearStage.h"
#include "Game.h"

ClearStage::ClearStage(float resultTime)
	:UIScreen(D2D.CreateRect(Vector2(0.5f, 0.65f), 0.2f, 0.1f))
{
	GAME.SetState(Game::State::Paused);
	GAME.GetInputSystem()->SetRelativeMode(false);

	Text* title = new Text(L"Mission Complete", Vector2(0.5f, 0.2f));
	title->SetFormat(D2D.g_titleFormat);
	SetTitle(title);

	auto temp = math::ConvertToTimeRotation(resultTime);
	m_resultTime.reset(new Text(L"TIME "+temp, Vector2(0.5f, 0.4f)));
	
	AddButton(L"Replay", [this]() { Close(); GAME.ReloadScene(); });
	AddButton(L"Quit", [this]() {GAME.SetState(Game::State::Quit); });
}

void ClearStage::Draw()
{
	m_resultTime->Draw();

	UIScreen::Draw();
}