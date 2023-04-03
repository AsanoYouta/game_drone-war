#include "Slider.h"

Slider::Slider(const Vector2& pos, float maxLength, float height)
	:m_pos(pos)
	,m_height(height)
	,m_maxLength(maxLength)
{
	m_brush = D2D.GetDefBrush();
	m_frameRect = D2D.CreateRectSide(pos, maxLength, height);
}

void Slider::Draw(float param)
{
	D2D_POINT_2F point = D2D1::Point2F(
		m_pos.x * D3D.m_viewPort.Width,
		m_pos.y * D3D.m_viewPort.Height);
	D2D.GetRT()->SetTransform(
		D2D1::Matrix3x2F::Rotation(m_rotation,point));

	D2D.GetRT()->DrawRectangle(m_frameRect, m_brush.Get());

	float curLength = m_maxLength * param;
	m_fillRect = D2D.CreateRectSide(m_pos, curLength, m_height);

	D2D.GetRT()->FillRectangle(m_fillRect, m_brush.Get());

	//‰ñ“]‚ð–ß‚·
	D2D.GetRT()->SetTransform(D2D1::Matrix3x2F::Identity());
}