#include "GameOver.h"
#include "Game.h"

GameOver::GameOver()
	:UIScreen(D2D.CreateRect(0.4f, 0.5f, 0.6f, 0.6f))
{
	GAME.GetInputSystem()->SetRelativeMode(false);

	Text* text = new Text(L"Game Over", Vector2(0.5f, 0.2f));
	text->SetFormat(D2D.g_titleFormat);
	SetTitle(text);

	AddButton(L"Retry", [this]() 
		{
			Close(); 
			GAME.LoadScene("Battle");
			DEBUG_LOG("retry\n");
		});
	AddButton(L"Quit", [this]() {GAME.SetState(Game::State::Quit); });
}
