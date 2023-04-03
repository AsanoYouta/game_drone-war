#pragma once
#include "UIScreen.h"

class Slider
{
public:
	Slider(const Vector2& pos, float maxLength, float height);

	void SetBrush(ComPtr<ID2D1SolidColorBrush> brush) { m_brush = brush; }
	void SetPos(const Vector2& pos) { m_pos = pos; }
	void SetRotation(float degree) { m_rotation = degree; }
	const Vector2& GetPos() { return m_pos; }
	float GetRotation() { return m_rotation; }

	//スライダーゲージを描画(0~1を指定)
	void Draw(float param);

private:
	ComPtr<ID2D1SolidColorBrush> m_brush;
	Vector2 m_pos;
	float m_height;
	float m_maxLength;
	float m_rotation;
	D2D1_RECT_F m_frameRect;
	D2D1_RECT_F m_fillRect;
};

