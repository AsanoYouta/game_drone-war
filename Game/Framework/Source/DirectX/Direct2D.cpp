#include "Direct2D.h"
#include "Source/Game/Game.h"
#include "Source/Game/UIScreen.h"
#include "Source/Math/mymath.h"

Direct2D::Direct2D()
{}

bool Direct2D::Initialize(HWND hWnd)
{
	m_hwnd = hWnd;

	//ファクトリ生成
	ID2D1Factory* factory = NULL;
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&factory);
	if (FAILED(hr)) 
	{
		DEBUG_LOG("Failed to Create ID2D1Factory\n");
		return false;
	}

	//バックバッファ取得
	D3D.m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_backBuffer));
	//レンダーターゲット生成
	FLOAT dpiX;
	FLOAT dpiY;
	dpiX = static_cast<FLOAT>(GetDpiForWindow(hWnd));
	dpiY = static_cast<FLOAT>(GetDpiForWindow(hWnd));
	D2D1_RENDER_TARGET_PROPERTIES props = 
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT, 
			D2D1::PixelFormat
			(
				DXGI_FORMAT_UNKNOWN, 
				D2D1_ALPHA_MODE_PREMULTIPLIED
			), 
			dpiX, 
			dpiY);
	factory->CreateDxgiSurfaceRenderTarget(m_backBuffer.Get(), &props, &m_renderTarget);

	//デフォルトカラー定義
	g_defColor = D2D1::ColorF(0.1f, 1.0f, 0.03f, 1.0f);
	
	//デフォルトブラシ生成
	if (FAILED(m_renderTarget->CreateSolidColorBrush(
		g_defColor,
		&m_defBrush
	)))
	{
		DEBUG_LOG("Failed to CreateSolidColorBrush\n");
		return false;
	}

	//DWriteファクトリ生成
	if (FAILED(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory5),
		&m_dWriteFactory			//注意
	))) 
	{
		DEBUG_LOG("Failed to Create DWriteFactory\n");
		return false;
	}

	ComPtr<IDWriteFontSetBuilder1> fontSetBuilder;
	if (FAILED(m_dWriteFactory->CreateFontSetBuilder(&fontSetBuilder))) {
		DEBUG_LOG("Failed to CreateFontSetBuilder\n");
	}

	//フォントファイル/フォントコレクション作成
	if (FAILED(m_dWriteFactory->CreateFontFileReference(
		L"Assets/Fonts/x14y24pxHeadUpDaisy.ttf", nullptr, &m_defFontFile))) {
		DEBUG_LOG("Failed to CreateFontFileReference\n");
	}
	if (FAILED(fontSetBuilder->AddFontFile(m_defFontFile.Get()))) {
		DEBUG_LOG("Failed AddFontFile\n");
	}
	if (FAILED(fontSetBuilder->CreateFontSet(&m_fontSet))) {
		DEBUG_LOG("Failed to CreateFontSet\n");
	}
	if (FAILED(m_dWriteFactory->CreateFontCollectionFromFontSet(m_fontSet.Get(), &m_fontCollection))) {
		DEBUG_LOG("Failed to CreateFontCollection\n");
	}

	//デフォルトテキストフォーマット作成
	if (FAILED(m_dWriteFactory->CreateTextFormat(
		L"x14y24pxHeadUpDaisy",				//フォントファミリーネーム
		m_fontCollection.Get(),				//フォントコレクション
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		52.0f,
		L"en-us",
		&m_defTextFormat
	))) 
	{
		DEBUG_LOG("Failed to Create TextFormat\n");
		return false;
	}
	if (FAILED(m_defTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER))) {
		DEBUG_LOG("Failed to SetTextAlignment\n");
		return false;
	}
	if (FAILED(m_defTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER))) {
		DEBUG_LOG("Failed to SetParagraphAlignment\n");
		return false;
	}

	if (FAILED(CoInitialize(NULL))) {
		DEBUG_LOG("Failed to Init Co\n");
		return false;
	}

	//その他ブラシ,テキストフォーマット作成
	if (FAILED(m_dWriteFactory->CreateTextFormat(
		L"x14y24pxHeadUpDaisy",				//フォントファミリーネーム
		m_fontCollection.Get(),				//フォントコレクション					
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		72.0f,
		L"en-us",
		&g_titleFormat
	))){
		DEBUG_LOG("Failed to Create TextFormat\n");
		return false;
	}
	g_titleFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	g_titleFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	if (FAILED(m_dWriteFactory->CreateTextFormat(
		L"x14y24pxHeadUpDaisy",				//フォントファミリーネーム
		m_fontCollection.Get(),				//フォントコレクション					
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		25.0f,
		L"en-us",
		&g_hudFormat
	))) {
		DEBUG_LOG("Failed to Create TextFormat\n");
		return false;
	}
	g_hudFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	g_hudFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	if (FAILED(m_dWriteFactory->CreateTextFormat(
		L"x14y24pxHeadUpDaisy",				//フォントファミリーネーム
		m_fontCollection.Get(),				//フォントコレクション					
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		21.0f,
		L"en-us",
		&g_hudFormat2
	))) {
		DEBUG_LOG("Failed to Create TextFormat\n");
		return false;
	}
	g_hudFormat2->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	g_hudFormat2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	if (FAILED(m_dWriteFactory->CreateTextFormat(
		L"x14y24pxHeadUpDaisy",				//フォントファミリーネーム
		m_fontCollection.Get(),				//フォントコレクション					
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		21.0f,
		L"en-us",
		&g_hudFormat3
	))) {
		DEBUG_LOG("Failed to Create TextFormat\n");
		return false;
	}
	g_hudFormat3->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	g_hudFormat3->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	m_renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Orange),
		&g_onButtonBrush);
	m_renderTarget->CreateSolidColorBrush(
		g_defColor,
		&g_offButtonBrush);
	m_renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&g_redBrush);
	m_renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::DarkGray),
		&g_grayBrush);

	return true;
}

void Direct2D::Draw()
{
	m_renderTarget->BeginDraw();

	for (auto ui : GAME.GetUIStack())
	{
		ui->Draw();
	}

	m_renderTarget->EndDraw();
}

D2D1_RECT_F Direct2D::CreateRect(float left, float top, float right, float bottom)
{
	auto vp = D3D.m_viewPort;
	D2D1_RECT_F rect = D2D1::RectF(
		left * vp.Width,
		top * vp.Height,
		right * vp.Width,
		bottom * vp.Height);

	return rect;
}
D2D1_RECT_F Direct2D::CreateRect(Vector2 pos, float width, float height)
{
	pos.x *= D3D.m_viewPort.Width;
	pos.y *= D3D.m_viewPort.Height;
	width *= D3D.m_viewPort.Width;
	height *= D3D.m_viewPort.Height;

	auto ret = D2D1::RectF(
		pos.x - width / 2,
		pos.y - height / 2,
		pos.x + width / 2,
		pos.y + height / 2
	);

	return ret;
}

D2D1_RECT_F Direct2D::CreateRectSide(Vector2 pos, float width, float height)
{
	pos.x *= D3D.m_viewPort.Width;
	pos.y *= D3D.m_viewPort.Height;
	width *= D3D.m_viewPort.Width;
	height *= D3D.m_viewPort.Height;

	//left基準
	auto ret = D2D1::RectF(
		pos.x,
		pos.y - height / 2,
		pos.x + width,
		pos.y + height / 2
	);

	return ret;
}
