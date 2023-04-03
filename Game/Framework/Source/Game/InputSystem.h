#pragma once
#include "framework.h"
#include "Source/Math/mymath.h"

enum KeyCode
{
	VK_A = 0x41,
	VK_D = 0x44,
	VK_F = 0x46,
	VK_G = 0x57,
	VK_S = 0x53,
	VK_W = 0x57
};

enum ButtonState
{
	None,
	Pressed,
	Released,
	Held
};

class KeyboardState
{
public:
	friend class InputSystem;
	bool GetKeyValue(BYTE keyCode) const;
	ButtonState GetKeyState(BYTE keyCode) const;
private:
	BYTE m_curKeys[256];
	BYTE m_prevKeys[256];
};

class MouseState
{
public:
	friend class InputSystem;

	const POINT& GetPosition() const { return m_mousePos; }
	const Vector2& GetRelativeVec() const { return m_relativeVec; }

private:
	POINT m_mousePos;
	POINT m_prevMousePos;
	Vector2 m_relativeVec;
	const float m_maxMouseVec = 300;
	bool m_isRelative;
};

struct InputState
{
	KeyboardState Keyboad;
	MouseState mouse;
};

class InputSystem
{
public:
	bool Initialize(HWND hWnd);
	void Shutdown();
	void Update(MSG msg);

	const InputState& GetState() const { return m_state; }
	void SetRelativeMode(bool val);

private:
	void UpdateClipRect();

	InputState m_state;
	HWND m_hwnd;
	RECT m_clipRect;
};

