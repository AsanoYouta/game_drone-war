#pragma once
#include "DirectX.h"
#include "framework.h"

class RenderPass
{
public:
	RenderPass(const std::wstring& shaderName, const std::string& vsEntry, const std::string& psEntry);
	~RenderPass();

	ID3D11ShaderResourceView** GetSrv() { return m_texSrv.GetAddressOf(); }
	void SetRTV(const ComPtr<ID3D11RenderTargetView>& rtv) { m_rtv = rtv; }

	void DrawPass(ID3D11ShaderResourceView* const* srv);
	void DrawPass(ID3D11ShaderResourceView* const* srv[], size_t numTexs);

private:
	ComPtr<ID3D11Texture2D> m_renderTexture;
	ComPtr<ID3D11RenderTargetView> m_rtv;
	ComPtr<ID3D11ShaderResourceView> m_texSrv;
	ComPtr<ID3D11SamplerState> m_samplerState;

	class Shader* m_shader;
};

