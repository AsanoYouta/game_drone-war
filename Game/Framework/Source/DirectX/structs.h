#pragma once
#include<DirectXMath.h>
#include<d3d11.h>
#include<vector>
using namespace DirectX;

//頂点レイアウト
struct Vertex3D
{
	XMFLOAT3 Pos;
	XMFLOAT3 Nor;
	XMFLOAT2 Tex;
};
//インスタンスごとに指定するデータ
struct InstanceData
{
	XMFLOAT4X4 World;
	XMFLOAT4 Color;
};

struct VertexFBO
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};
const VertexFBO VERTS_RECT[4] =
{
	{XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)},
	{XMFLOAT3(-1.0f,  1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)},
	{XMFLOAT3( 1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)},
	{XMFLOAT3( 1.0f,  1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)},
};

namespace vertLayout
{
	//3D用頂点レイアウト
	const std::vector<D3D11_INPUT_ELEMENT_DESC> VERTEX3D =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//インスタンスごとのワールド行列
		//float4x4
		{ "MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		//インスタンスごとの色
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	//FBO用頂点レイアウト
	const std::vector<D3D11_INPUT_ELEMENT_DESC> FBO =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
}

//シェーダーに設定するコンスタントバッファ
//注意:16byte単位で設定する
struct ConstantMatrixBuffer
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

struct DirectionalLight
{
	XMFLOAT4 dir;       //(x,y,z,0)
	XMFLOAT4 diffuse;   //(r,g,b,0)
	XMFLOAT4 specColor; //(r,g,b,0)
};

struct Material
{
	XMFLOAT4 diffuse;  //(r,g,b,0)
	XMFLOAT4 specular; //(r,g,b,specPow)
	XMFLOAT4 ambient;  //(r,g,b,0)
	XMFLOAT4 emissive; //(r,g,b,0) 加算される
};

struct ConstantLightBuffer
{
	XMFLOAT4 eyePos;           //(x,y,z,0)
	XMFLOAT4 ambient;          //(x,y,z,0)
	DirectionalLight dirLight; 
	Material mat;
};

struct ConstantFBOBuffer
{
	XMFLOAT2 viewPort;   //(width, height)
	XMFLOAT2 options;    //(offset, intensity)
	float gausWeight[16]; //64byte
};

namespace material
{
	const Material White
	{
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f),
		XMFLOAT4(0.2f, 0.2f, 0.2f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)
	};

	const Material RoughWhite
	{
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		XMFLOAT4(0.2f, 0.2f, 0.2f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)
	};

	const Material BrightWhite
	{
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT4(0.6f, 0.6f, 0.6f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)
	};

	const Material EmmisiveWhite
	{
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f)
	};

	const Material Skybox
	{
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT4(100.0f, 100.0f, 100.0f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)
	};

	const Material Smoke
	{
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)
	};

	const Material Bullet
	{
		XMFLOAT4(1.1f,  0.68f, 0.4f, 0.0f),
		XMFLOAT4(1.1f,  0.68f, 0.4f, 20.0f),
		XMFLOAT4(1.1f,  0.68f, 0.4f, 0.0f),
		XMFLOAT4(1.1f,  0.68f, 0.4f, 0.0f)
	};

	const Material MetalBlack
	{
		XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 10.0f),
		XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)
	};

	const Material Explode
	{
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.2f, 0.67f, 0.5f, 0.0f)
	};

	const Material Flash
	{
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT4(0.9f, 0.9f, 0.0f, 0.0f),
		XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f)
	};
}