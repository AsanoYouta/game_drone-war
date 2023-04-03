#pragma once
#include "framework.h"
#include "Source/DirectX/DirectX.h"

class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);
	void Unload();

	void SetSrvAndSampler(ID3D11SamplerState* const* state);

private:
	//‰æ‘œ“Ç‚İæ‚èƒnƒ“ƒhƒ‹
	ComPtr<ID3D11ShaderResourceView> m_srv = nullptr;
	//‰æ‘œî•ñ
	DirectX::TexMetadata m_info = {};
};

