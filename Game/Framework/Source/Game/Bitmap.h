#pragma once
#include "framework.h"
#include "Source/DirectX/DirectX.h"

class Bitmap
{
public:
	Bitmap(std::wstring fileName, D2D1_RECT_F rect);
	//���K���X�N���[�����W���w��(0~1)
	Bitmap(std::wstring fileName, Vector2 pos, float scale = 1.0f);
	~Bitmap();

	bool Load(std::wstring fileName);
	void Unload();
	//�摜�{���̃s�N�Z���T�C�Y�ɑΉ�����RECT�𐶐�
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

