#pragma once
#include "DirectX.h"
#include <d2d1.h>
#include <dwrite.h>
#include <dwrite_3.h>
#include "Source/Game/Shader.h"
#pragma comment(lib,"d2d1.lib")    
#pragma comment(lib,"dwrite.lib")

class Direct2D
{
public:
	bool Initialize(HWND hWnd);
	void Shutdown();

	void Draw();

	ComPtr<IDWriteFactory> GetDWriteFactory() { return m_dWriteFactory; }
	ComPtr<ID2D1SolidColorBrush> GetDefBrush() { return m_defBrush; }
	ComPtr<IDWriteTextFormat> GetDefTextFormat() { return m_defTextFormat; }
	ComPtr<ID2D1RenderTarget> GetRT() { return m_renderTarget; }

	//正規化数値から表示領域を生成(0~1を指定する　中心が0.5)
	D2D1_RECT_F CreateRect(float left, float top, float right, float bottom);
	//posを中心に表示領域を作成(width,heightは0~1を指定)
	D2D1_RECT_F CreateRect(class Vector2 pos, float width, float height);
	//posを左端とした表示領域を作成(width,heightは0~1を指定)
	D2D1_RECT_F CreateRectSide(class Vector2 pos, float width, float height);

	D2D1_COLOR_F g_defColor;
	ComPtr<ID2D1SolidColorBrush> g_onButtonBrush;
	ComPtr<ID2D1SolidColorBrush> g_offButtonBrush;
	ComPtr<ID2D1SolidColorBrush> g_redBrush;
	ComPtr<ID2D1SolidColorBrush> g_grayBrush;
	ComPtr<IDWriteTextFormat> g_titleFormat;
	ComPtr<IDWriteTextFormat> g_hudFormat;
	ComPtr<IDWriteTextFormat> g_hudFormat2;
	ComPtr<IDWriteTextFormat> g_hudFormat3;

private:
	HWND m_hwnd;
	ComPtr<ID2D1RenderTarget> m_renderTarget;
	ComPtr<IDXGISurface> m_backBuffer;
	ComPtr<ID2D1SolidColorBrush> m_defBrush;
	ComPtr<IDWriteFactory5> m_dWriteFactory;
	ComPtr<IDWriteTextFormat> m_defTextFormat;
	ComPtr<IDWriteFontFile> m_defFontFile;
	ComPtr<IDWriteFontSet> m_fontSet;
	ComPtr<IDWriteFontCollection1> m_fontCollection;
	Shader* m_spriteShader;

//----------------------------------------------------
//シングルトン
	static inline Direct2D* instance;
	Direct2D();
public:
	static void CreateInstance()
	{
		DeleteInstance();
		instance = new Direct2D();
	}
	static void DeleteInstance()
	{
		if (instance != nullptr) {
			delete instance;
			instance = nullptr;
		}
	}
	static Direct2D& GetInstance()
	{
		return *instance;
	}
};

#define D2D Direct2D::GetInstance()

