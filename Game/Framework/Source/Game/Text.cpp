#include "Text.h"

Text::Text(std::wstring text, Vector2 pos, ComPtr<IDWriteTextFormat> format)
	:m_drawRect(false)
{
	m_text = text;
	m_brush = D2D.GetDefBrush();
	m_textFormat = format;
	m_pos = pos;
	SetRectFromPos(pos);
	m_frameRect = m_textRect;
}

Text::~Text()
{
}

void Text::Draw()
{
	//RectXV
	SetRectFromPos(m_pos);

	D2D.GetRT()->DrawTextW(
		m_text.c_str(),
		m_text.size(),
		m_textFormat.Get(),
		m_textRect,
		m_brush.Get(),
		D2D1_DRAW_TEXT_OPTIONS_NONE
	);

	if (m_drawRect)
	{
		D2D.GetRT()->DrawRectangle(m_frameRect, m_brush.Get());
	}
}

void Text::SetRectFromPos(Vector2 pos)
{
	pos.x *= D3D.m_viewPort.Width;
	pos.y *= D3D.m_viewPort.Height;

	auto sizey = m_textFormat->GetFontSize();
	auto sizex = sizey * m_text.size();
	m_textRect = D2D1::RectF(
		pos.x - sizex / 2,
		pos.y - sizey / 2,
		pos.x + sizex / 2,
		pos.y + sizey / 2
	);
}
