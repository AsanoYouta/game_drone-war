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
	//画像読み取りハンドル
	ComPtr<ID3D11ShaderResourceView> m_srv = nullptr;
	//画像情報
	DirectX::TexMetadata m_info = {};
};

