#pragma once
#include "framework.h"
#include "Source/DirectX/DirectX.h"

class Bitmap
{
public:
	Bitmap(std::wstring fileName, D2D1_RECT_F rect);
	//正規化スクリーン座標を指定(0~1)
	Bitmap(std::wstring fileName, Vector2 pos, float scale = 1.0f);
	~Bitmap();

	bool Load(std::wstring fileName);
	void Unload();
	//画像本来のピクセルサイズに対応したRECTを生成
	void SetNaturalRect(Vector2 pos);
	void SetScale(float scale) { m_scale = scale; m_updateRect = true; }
	void SetRotation(float degree) { m_rotation = degree; }
	const Vector2& GetPos() const { return m_pos; }
	float GetScale() const { return m_scale; }

	void Draw();

private:
	ComPtr<ID2D1Bitmap> m_bitmap;
	D2D1_RECT_F m_rect;
	Vector2 m_pos;
	float m_scale;
	float m_rotation;
	bool m_updateRect;
};

