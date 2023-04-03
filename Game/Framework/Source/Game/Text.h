#pragma once
#include "framework.h"
#include "Source/DirectX/DirectX.h"

class Text
{
public:
	//正規化スクリーン座標を指定(0~1)
	Text(std::wstring text, Vector2 pos,
		ComPtr<IDWriteTextFormat> format = D2D.GetDefTextFormat());
	~Text();

	//スクリーン座標からRECT作成
	void SetRectFromPos(Vector2 pos);

	void SetText(std::wstring text) { m_text = text; }
	void SetBrush(ComPtr<ID2D1SolidColorBrush> brush) { m_brush = brush; }
	void SetFormat(ComPtr<IDWriteTextFormat> format) { m_textFormat = format; }
	void SetDrawRect(bool draw) { m_drawRect = draw; }
	void SetFrameRect(D2D1_RECT_F frame) { m_frameRect = frame; }
	Vector2 GetPos() { return m_pos; }
	D2D1_RECT_F GetTextRect() { return m_textRect; }

	void Draw();

private:
	ComPtr<ID2D1SolidColorBrush> m_brush;
	ComPtr<IDWriteTextFormat> m_textFormat;
	std::wstring m_text;
	D2D1_RECT_F m_textRect;
	D2D1_RECT_F m_frameRect;
	Vector2 m_pos;
	bool m_drawRect;
};

