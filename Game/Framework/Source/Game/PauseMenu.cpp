#include "PauseMenu.h"
#include "Game.h"
#include "Scene.h"
#include "HUD.h"

PauseMenu::PauseMenu()
	:UIScreen(D2D.CreateRect(0.4f, 0.4f, 0.6f, 0.5f))
	,m_pauseCount(0)
{
	GAME.SetState(Game::State::Paused);
	GAME.GetInputSystem()->SetRelativeMode(false);

	Text* text = new Text(L"Pause", Vector2(0.5f, 0.2f));
	text->SetFormat(D2D.g_titleFormat);
	SetTitle(text);

	//Bitmap* bitmap = new Bitmap(L"Assets/Textures/Default.png", Vector2(0.5f, 0.5f), 1.0f);
	D2D_RECT_F rect = D2D.CreateRect(Vector2(0.5f, 0.47f), 0.27f, 0.70f);
	SetBackBrush(D2D.g_grayBrush);
	//SetBackground(rect);

	AddButton(L"Resume", [this]() { Close(); });
	AddButton(L"Restart", [this]() { GAME.ReloadScene(); });
	AddButton(L"Quit", [this]() { GAME.SetState(Game::State::Quit); });
}

PauseMenu::~PauseMenu()
{
	GAME.SetState(Game::State::GamePlay);
	GAME.GetInputSystem()->SetRelativeMode(true);
}

void PauseMenu::ProcessInput(const InputState& state)
{
	if (state.Keyboad.GetKeyState(VK_ESCAPE) == Pressed && 
		m_pauseCount > 0)
	{
		Close();
		DEBUG_LOG("ClosePauseMenu\n");
	}

	UIScreen::ProcessInput(state);
}

void PauseMenu::Update(float deltaTime)
{
	m_pauseCount++;
}