#include "Bitmap.h"
#include <wincodec.h>

Bitmap::Bitmap(std::wstring fileName, D2D1_RECT_F rect)
{
    Load(fileName);
    m_rect = rect;
}
Bitmap::Bitmap(std::wstring fileName, Vector2 pos, float scale)
{
    Load(fileName);
    m_scale = scale;
    m_pos = pos;
    SetNaturalRect(pos);
}

Bitmap::~Bitmap()
{
    Unload();
}

bool Bitmap::Load(std::wstring fileName)
{
    ComPtr<IWICImagingFactory> factory;
    if (FAILED(CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&factory))))
    {
        DEBUG_LOG("Failed to Create CoInstance\n");
        return false;
    }

    //BitMapDecoder作成
    ComPtr<IWICBitmapDecoder> decoder;
    if (FAILED(factory->CreateDecoderFromFilename(
        fileName.c_str(),
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnDemand,
        &decoder)))
    {
        DEBUG_LOG("Failed to Create DecoderFromFilename : %ls\n", fileName.c_str());
        return false;
    }
    //イメージからフレームを取得
    ComPtr<IWICBitmapFrameDecode> frame;
    decoder->GetFrame(0, &frame);

    // 32bit RGBAに変換(Direct2Dで使用できる形式にする)
    ComPtr<IWICFormatConverter> converter;
    if (FAILED(factory->CreateFormatConverter(&converter))) {
        DEBUG_LOG("Failed to CreateFormatConverter\n");
        return false;
    }
    if (FAILED(converter->Initialize(
        frame.Get(),
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        NULL,
        0.0f,
        WICBitmapPaletteTypeCustom)))
    {
        DEBUG_LOG("Failed to converter Init\n");
        return false;
    }

    if (FAILED(D2D.GetRT()->CreateBitmapFromWicBitmap(
        converter.Get(),
        NULL,
        &m_bitmap)))
    {
        DEBUG_LOG("Failed to CreateBitmap\n");
        return false;
    }

    return true;
}

void Bitmap::Unload()
{
    m_bitmap.Reset();
}

void Bitmap::SetNaturalRect(Vector2 pos)
{
    m_pos = pos;
    pos.x *= D3D.m_viewPort.Width;
    pos.y *= D3D.m_viewPort.Height;
    auto size = m_bitmap->GetPixelSize();
    size.width = UINT32(size.width * m_scale);
    size.height = UINT32(size.height * m_scale);
    m_rect = D2D1::RectF(
        pos.x - size.width / 2,
        pos.y - size.height / 2,
        pos.x + size.width / 2,
        pos.y + size.height/ 2
    );
}

void Bitmap::Draw()
{
    D2D_POINT_2F center = D2D1::Point2F(
        m_pos.x*D3D.m_viewPort.Width,
        m_pos.y*D3D.m_viewPort.Height);
    D2D.GetRT()->SetTransform(
        D2D1::Matrix3x2F::Rotation(
            m_rotation,
            center));

    if (m_updateRect)
    {
        SetNaturalRect(m_pos);
        m_updateRect = false;
    }

    D2D.GetRT()->DrawBitmap(m_bitmap.Get(), m_rect);

    //回転を戻す
    D2D.GetRT()->SetTransform(D2D1::Matrix3x2F::Identity());
}
