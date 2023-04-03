#include "Texture.h"

Texture::Texture()
{}

Texture::~Texture()
{
}

bool Texture::Load(const std::string& fileName)
{
	//�}���`�o�C�g�����񂩂烏�C�h������֕ϊ�
	setlocale(LC_CTYPE, "jpn");
	wchar_t wFilename[256];
	size_t ret;
	mbstowcs_s(&ret, wFilename, fileName.c_str(), 256);

	auto image = std::make_unique<ScratchImage>();
	if (FAILED(LoadFromWICFile(wFilename, WIC_FLAGS_NONE, &m_info, *image))) {
		m_info = {};
		return false;
	}

	//�~�b�v�}�b�v�̐���
	if (m_info.mipLevels == 1)
	{
		auto mipChain = std::make_unique<ScratchImage>();
		if (SUCCEEDED(GenerateMipMaps(image->GetImages(), image->GetImageCount(),
			image->GetMetadata(), TEX_FILTER_DEFAULT, 0, *mipChain)))
		{
			image = std::move(mipChain);
		}
	}

	if (FAILED(CreateShaderResourceView(D3D.GetDevice().Get(), image->GetImages(),
		image->GetImageCount(), m_info, &m_srv)))
	{
		DEBUG_LOG("Failed to Create ShaderResourceView");
		m_info = {};
		return false;
	}

	return true;
}

void Texture::Unload()
{
	m_srv.Reset();
}

void Texture::SetSrvAndSampler(ID3D11SamplerState*const* state)
{
	// �T���v���[�X�e�[�g�Z�b�g
	D3D.GetDeviceContext()->VSSetSamplers(0, 1, state);
	D3D.GetDeviceContext()->PSSetSamplers(0, 1, state);
	//SRV�Z�b�g
	D3D.GetDeviceContext()->PSSetShaderResources(0, 1, m_srv.GetAddressOf());
}
