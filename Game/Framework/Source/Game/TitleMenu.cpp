#include "TitleMenu.h"
#include "Game.h"

TitleMenu::TitleMenu()
	:UIScreen(D2D.CreateRect(Vector2(0.5f, 0.65f), 0.2f, 0.1f))
{
	Text* text = new Text(L"Drone War", Vector2(0.5f, 0.2f));
	text->SetFormat(D2D.g_titleFormat);
	SetTitle(text);

	AddButton(L"Start", [this]() {GAME.LoadScene("Battle"); });
	AddButton(L"Quit", [this]() {GAME.SetState(Game::State::Quit); });
}

TitleMenu::~TitleMenu()
{

}