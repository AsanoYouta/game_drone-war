#include "Shader.h"

Shader::Shader()
	: m_pixelShader(nullptr)
	, m_vertexShader(nullptr)
	, m_topology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{}

Shader::~Shader()
{}

void Shader::SetShaders()
{
	D3D.GetDeviceContext()->IASetPrimitiveTopology(m_topology);

	D3D.GetDeviceContext()->IASetInputLayout(m_inputLayout.Get());
	D3D.GetDeviceContext()->VSSetShader(m_vertexShader.Get(), 0, 0);
	D3D.GetDeviceContext()->PSSetShader(m_pixelShader.Get(), 0, 0);
}

bool Shader::Load(const std::wstring& vsName, const std::wstring& psName,
	const std::string& vsEntry, const std::string& psEntry,
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& vertexLayout,
	D3D_PRIMITIVE_TOPOLOGY topology)
{
	if (!CompileVertexShader(vsName.c_str(), vsEntry, vertexLayout) || 
		!CompilePixelShader(psName.c_str(), psEntry)) 
	{
		return false;
	};
	m_topology = topology;
	return true;
}

void Shader::Unload()
{
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_vertexShader.Reset();
}

bool Shader::CompileVertexShader(
	const LPCWSTR& fileName, 
	const std::string& vsEntry, 
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& vertexLayout)
{
	ComPtr<ID3DBlob> compiledVS;
	auto hr = D3DCompileFromFile(fileName, nullptr, nullptr,
		vsEntry.c_str(), "vs_5_0", 0, 0, &compiledVS, nullptr);
	if (FAILED(hr)) {
		DEBUG_LOG("VertexShader Compilation Failed : %s\n", vsEntry.c_str());
		return false;
	}

	//頂点シェーダー作成
	if (FAILED(D3D.GetDevice()->CreateVertexShader(compiledVS->GetBufferPointer(),
		compiledVS->GetBufferSize(), nullptr, &m_vertexShader)))
	{
		DEBUG_LOG("VertexShader Creation Failed : %s\n", vsEntry.c_str());
		return false;
	}

	// 頂点インプットレイアウト作成
	if (FAILED(D3D.GetDevice()->CreateInputLayout(&vertexLayout[0], (UINT)vertexLayout.size(),
		compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(),
		&m_inputLayout)))
	{
		DEBUG_LOG("VertexInputLayout Creation Failed : %s\n", vsEntry.c_str());
		return false;
	}

	return true;
}

bool Shader::CompilePixelShader(const LPCWSTR& fileName, const std::string& psEntry)
{
	ComPtr<ID3DBlob> compiledPS;
	auto hr = D3DCompileFromFile(fileName, nullptr, nullptr,
		psEntry.c_str(), "ps_5_0", 0, 0, &compiledPS, nullptr);
	if (FAILED(hr)) {
		DEBUG_LOG("PixelShader Compilation Failed : %s\n", psEntry.c_str());
		return false;
	}

	// ピクセルシェーダー作成
	if (FAILED(D3D.GetDevice()->CreatePixelShader(compiledPS->GetBufferPointer(),
		compiledPS->GetBufferSize(), nullptr, &m_pixelShader)))
	{
		DEBUG_LOG("PixelShader Creation Failed : %s\n", psEntry.c_str());
		return false;
	}

	return true;
}
