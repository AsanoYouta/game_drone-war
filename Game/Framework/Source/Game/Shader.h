#pragma once
#include "framework.h"
#include "Source/DirectX/DirectX.h"

class Shader
{
public:
	Shader();
	~Shader();

	bool Load(const std::wstring& vsName, const std::wstring& psName, 
		const std::string& vsEntry, const std::string& psEntry,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& vertexLayout,
		D3D_PRIMITIVE_TOPOLOGY topology);
	void Unload();
	void SetShaders();

private:
	bool CompileVertexShader(const LPCWSTR& fileName, const std::string& vsEntry, const std::vector<D3D11_INPUT_ELEMENT_DESC>& vertexLayout);
	bool CompilePixelShader(const LPCWSTR& fileName, const std::string& psEntry);

	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;
	ComPtr<ID3D11InputLayout> m_inputLayout;

	std::vector<D3D11_INPUT_ELEMENT_DESC> m_vertLayout;
	D3D_PRIMITIVE_TOPOLOGY m_topology;
};

