#include "InputSystem.h"

bool InputSystem::Initialize(HWND hWnd)
{
	//マウスカーソルをクライアント領域内に制限
	UpdateClipRect();

	InputSystem::SetRelativeMode(false);
	m_state.mouse.m_relativeVec.x = 0;
	m_state.mouse.m_relativeVec.y = 0;

	memset(m_state.Keyboad.m_curKeys, 0, 256);
	memset(m_state.Keyboad.m_prevKeys, 0, 256);

	m_hwnd = hWnd;

	return true;
}

void InputSystem::Update(MSG msg)
{
	UpdateClipRect();

	if (m_state.mouse.m_isRelative && msg.message == WM_MOUSEMOVE) 
	{
		GetCursorPos(&m_state.mouse.m_mousePos); //スクリーン座標取得
		m_state.mouse.m_relativeVec.x = ((float)m_state.mouse.m_mousePos.x - (float)m_state.mouse.m_prevMousePos.x);
		m_state.mouse.m_relativeVec.y = ((float)m_state.mouse.m_mousePos.y - (float)m_state.mouse.m_prevMousePos.y);
		//マウス速度制限
		m_state.mouse.m_relativeVec.x = std::clamp(m_state.mouse.m_relativeVec.x, -m_state.mouse.m_maxMouseVec, m_state.mouse.m_maxMouseVec);
		m_state.mouse.m_relativeVec.y = std::clamp(m_state.mouse.m_relativeVec.y, -m_state.mouse.m_maxMouseVec, m_state.mouse.m_maxMouseVec);
		//マウスポイントをクライアント領域の中心にセンタリング
		RECT clientRect;
		GetClientRect(m_hwnd, &clientRect); //クライアント領域"の"座標取得
		tagPOINT point{ clientRect.right / 2, clientRect.bottom / 2 };
		ClientToScreen(m_hwnd, &point); //スクリーン座標へ変換
		SetCursorPos(point.x, point.y);
		m_state.mouse.m_prevMousePos = point; //クライアント領域の中心
	}
	else
	{
		m_state.mouse.m_relativeVec.x = 0;
		m_state.mouse.m_relativeVec.y = 0;
	}

	//キー状態更新
	for (int i = 0; i < 256; i++)
	{
		m_state.Keyboad.m_prevKeys[i] = m_state.Keyboad.m_curKeys[i];
	}

	if (!GetKeyboardState(m_state.Keyboad.m_curKeys)) 
	{
		DEBUG_LOG("Failed to GetKeyboardState\n");
	}
}

void InputSystem::UpdateClipRect()
{
	if (m_state.mouse.m_isRelative)
	{
		RECT clientRect;
		GetClientRect(m_hwnd, &clientRect);
		POINT pt;
		pt.x = clientRect.left;
		pt.y = clientRect.top;
		ClientToScreen(m_hwnd, &pt);
		m_clipRect.left = pt.x;
		m_clipRect.top = pt.y;
		pt.x = clientRect.right;
		pt.y = clientRect.bottom;
		ClientToScreen(m_hwnd, &pt);
		m_clipRect.right = pt.x;
		m_clipRect.bottom = pt.y;
		ClipCursor(&m_clipRect);
	}
	else
	{
		ClipCursor(NULL);
	}
}

void InputSystem::SetRelativeMode(bool val)
{
	if (m_state.mouse.m_isRelative == val)
		return; //ShowCursor内部カウント増減阻止

	m_state.mouse.m_isRelative = val;
	int count = 0;
	if (val) 
	{
		count = ShowCursor(false);
	}
	else 
	{
		count = ShowCursor(true);
	}
	UpdateClipRect();
}

void InputSystem::Shutdown()
{
	ClipCursor(NULL);
}

bool KeyboardState::GetKeyValue(BYTE keyCode) const
{
	if (m_curKeys[keyCode] & 0x80)
	{
		return true;
	}
	else
	{
		return false;
	}
}

ButtonState KeyboardState::GetKeyState(BYTE keyCode) const
{
	if (m_prevKeys[keyCode] & 0x80) //直前に押されている
	{
		if (GetKeyValue(keyCode))
		{
			return Held;
		}
		else 
		{
			return Released;
		}
	}
	else                            //直前に押されていない
	{
		if (GetKeyValue(keyCode))
		{
			return Pressed;
		}
		else
		{
			return None;
		}
	}
}
