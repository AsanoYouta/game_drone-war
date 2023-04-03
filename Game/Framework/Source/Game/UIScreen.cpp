#include "UIScreen.h"
#include "Game.h"
#include "AudioSystem.h"

Button::Button(const std::wstring name,
	std::function<void()> onClick,
	const D2D_RECT_F r)
	:m_isContain(false)
{
	auto vp = D3D.m_viewPort;
	m_text = new Text(name, 
		Vector2((r.left+(r.right-r.left)/2)/vp.Width, 
			    (r.top+(r.bottom-r.top)/2)/vp.Height));
	m_rect = r;
	m_onClick = onClick;
}

void Button::OnClick()
{
	//SE再生
	GAME.GetAudioSystem()->PlayEvent("event:/UI_Select");

	if (m_onClick) m_onClick();
}

bool Button::ContainsPoint(const POINT& point) const
{
	bool no = point.y < m_rect.top ||
		point.y > m_rect.bottom ||
		point.x < m_rect.left ||
		point.x > m_rect.right;
	return !no;
}

void Button::Draw(const POINT& pt)
{
	if (ContainsPoint(pt))
	{
		D2D.GetRT()->DrawRectangle(m_rect, D2D.g_onButtonBrush.Get());
		m_isContain = true;
	}
	else
	{
		D2D.GetRT()->DrawRectangle(m_rect, D2D.g_offButtonBrush.Get());
		m_isContain = false;
	}
	m_text->Draw();
}

UIScreen::UIScreen(D2D_RECT_F nextButtonRect)
	:m_state(Active)
	,m_backImage(nullptr)
{
	GAME.PushUI(this);
	DEBUG_LOG("uiPushed\n");
	m_nextButtonRect = nextButtonRect;
	m_brush = D2D.GetDefBrush();
}
UIScreen::UIScreen()
	:m_state(Active)
	,m_backImage(nullptr)
{
	GAME.PushUI(this);
	m_brush = D2D.GetDefBrush();
}

UIScreen::~UIScreen()
{
}

//継承先でも必ず実行
void UIScreen::ProcessInput(const InputState& state)
{
	if (state.Keyboad.GetKeyState(VK_LBUTTON) != Pressed)
		return;
	for (auto button : m_buttons)
	{
		if (button->GetIsContain())
		{
			button->OnClick();
			DEBUG_LOG("OnClick\n");
		}
	}
}

void UIScreen::Update(float deltaTime)
{
}

void UIScreen::Draw()
{
	if (m_backImage)
	{
		m_backImage->Draw();
	}

	D2D.GetRT()->FillRectangle(m_backRect, m_brush.Get());
	

	if (m_title)
	{
		m_title->Draw();
	}

	POINT pt;
	GetCursorPos(&pt); //スクリーン座標取得
	ScreenToClient(D3D.GetHWND(), &pt);
	for (auto button : m_buttons)
	{
		button->Draw(pt);
	}
}

void UIScreen::Close()
{
	m_state = Closing;
}

void UIScreen::AddButton(const std::wstring& name, std::function<void()> onClick)
{
	Button* b = new Button(name, onClick, m_nextButtonRect);
	m_buttons.emplace_back(b);

	float buttonHeight = m_nextButtonRect.bottom - m_nextButtonRect.top;
	m_nextButtonRect.top = m_nextButtonRect.bottom + 20.0f;
	m_nextButtonRect.bottom = m_nextButtonRect.top + buttonHeight;
}
