#include "RenderPass.h"
#include "Source/Game/Shader.h"

RenderPass::RenderPass(const std::wstring& shaderName, const std::string& vsEntry, const std::string& psEntry)
	:m_shader(nullptr)
{
	//オフスクリーン用テクスチャ作成
	D3D11_TEXTURE2D_DESC txDesc;
	ZeroMemory(&txDesc, sizeof(txDesc));
	txDesc.Width = UINT(D3D.m_viewPort.Width);
	txDesc.Height = UINT(D3D.m_viewPort.Height);
	txDesc.MipLevels = 1;
	txDesc.ArraySize = 1;
	txDesc.SampleDesc.Count = 1;
	txDesc.SampleDesc.Quality = 0;
	txDesc.Usage = D3D11_USAGE_DEFAULT;
	txDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	txDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	txDesc.CPUAccessFlags = 0;
	txDesc.MiscFlags = 0;
	if (FAILED(D3D.GetDevice()->CreateTexture2D(&txDesc, NULL, &m_renderTexture))) {
		DEBUG_LOG("Failed to CreateOffScTex\n");
	}
	//SRV作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = txDesc.MipLevels;
	D3D.GetDevice()->CreateShaderResourceView(m_renderTexture.Get(), &srvDesc, &m_texSrv);
	//オフスクリーン用RTV作成
	if (FAILED(D3D.GetDevice()->CreateRenderTargetView(m_renderTexture.Get(), NULL, &m_rtv))) {
		DEBUG_LOG("Failed to CreateOffScRTV\n");
	}

	//シェーダーロード
	m_shader = new Shader();
	m_shader->Load(
		shaderName,
		shaderName,
		vsEntry, psEntry,
		vertLayout::FBO,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);
}

void RenderPass::DrawPass(ID3D11ShaderResourceView* const* srv)
{
	float clearColor[4] = { 0.3f, 0.3f, 0.3f, 1.0f }; //TODO:activeSceneクリアカラー
	//バックバッファ描画設定
	D3D.m_deviceContext->OMSetRenderTargets(1, m_rtv.GetAddressOf(), D3D.m_depthStencilView.Get());
	D3D.m_deviceContext->ClearRenderTargetView(m_rtv.Get(), clearColor);
	D3D.m_deviceContext->ClearDepthStencilView(D3D.m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_shader->SetShaders();
	//srvセット
	D3D.m_deviceContext->PSSetSamplers(0, 1, D3D.m_samplerState.GetAddressOf());
	D3D.m_deviceContext->PSSetShaderResources(0, 1, srv);
	//コンスタントバッファセット
	D3D.m_deviceContext->UpdateSubresource(D3D.m_fboBuffer.Get(), 0, NULL, &D3D.m_cfb, 0, 0);
	D3D.m_deviceContext->PSSetConstantBuffers(0, 1, D3D.m_fboBuffer.GetAddressOf());
	//頂点バッファ設定
	UINT offset = 0;
	UINT stride = sizeof(VertexFBO);
	D3D.m_deviceContext->IASetVertexBuffers(0, 1, D3D.m_renderTexVB.GetAddressOf(), &stride, &offset);
	D3D.m_deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	//描画
	D3D.m_deviceContext->Draw(4, 0);
}

void RenderPass::DrawPass(ID3D11ShaderResourceView*const* srv[], size_t numTexs)
{
	float clearColor[4] = { 0.3f, 0.3f, 0.3f, 1.0f }; //TODO:activeSceneクリアカラー
	//バックバッファ描画設定
	D3D.m_deviceContext->OMSetRenderTargets(1, m_rtv.GetAddressOf(), D3D.m_depthStencilView.Get());
	D3D.m_deviceContext->ClearRenderTargetView(m_rtv.Get(), clearColor);
	D3D.m_deviceContext->ClearDepthStencilView(D3D.m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_shader->SetShaders();
	//srvセット
	D3D.m_deviceContext->PSSetSamplers(0, 1, D3D.m_samplerState.GetAddressOf());
	for (int i = 0; i < numTexs; i++)
	{
		D3D.m_deviceContext->PSSetShaderResources(i, i+1, srv[i]);
	}
	//コンスタントバッファセット
	D3D.m_deviceContext->UpdateSubresource(D3D.m_fboBuffer.Get(), 0, NULL, &D3D.m_cfb, 0, 0);
	D3D.m_deviceContext->PSSetConstantBuffers(0, 1, D3D.m_fboBuffer.GetAddressOf());
	//頂点バッファ設定
	UINT offset = 0;
	UINT stride = sizeof(VertexFBO);
	D3D.m_deviceContext->IASetVertexBuffers(0, 1, D3D.m_renderTexVB.GetAddressOf(), &stride, &offset);
	D3D.m_deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	//描画
	D3D.m_deviceContext->Draw(4, 0);
}
