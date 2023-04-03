#pragma once
#include "Source/Math/mymath.h"
#include "InputSystem.h"
#include "Text.h"
#include "Bitmap.h"
#include "Source/DirectX/DirectX.h"

class Button
{
public:
	Button(const std::wstring name,
		std::function<void()> onClick,
		const D2D_RECT_F rect);

	bool ContainsPoint(const POINT& point) const;
	void OnClick();
	void Draw(const POINT& pt);
	bool GetIsContain() const { return m_isContain; }

private:
	Text* m_text;
	Bitmap* m_background;
	D2D_RECT_F m_rect;
	std::function<void()> m_onClick;
	bool m_isContain;
};

class UIScreen
{
public:
	UIScreen(D2D_RECT_F nextButtonRect);
	UIScreen();
	virtual ~UIScreen();

	enum State {
		Active,
		Closing
	};
	State GetState() const { return m_state; }
	void Close();

	void SetTitle(Text* text) { m_title = text; }
	void SetBackBrush(ComPtr<ID2D1SolidColorBrush> brush) { m_brush = brush; }
	void SetBackground(Bitmap* bitmap) { m_backImage = bitmap; }
	void SetBackground(D2D1_RECT_F rect) { m_backRect = rect; }
	void AddButton(const std::wstring& name, std::function<void()> onClick);

	//メニュー画面、HUDなどの派生先で追加設定
	virtual void Update(float deltaTime);
	virtual void Draw();
	//継承先でも必ず実行する
	virtual void ProcessInput(const InputState& state);

protected:
	State m_state;
	Text* m_title;
	Bitmap* m_backImage;
	D2D1_RECT_F m_backRect;
	ComPtr<ID2D1SolidColorBrush> m_brush;
	std::vector<Button*> m_buttons;
	D2D_RECT_F m_nextButtonRect;
};

