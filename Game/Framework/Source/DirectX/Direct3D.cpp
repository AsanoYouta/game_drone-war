#include "Direct3D.h"
#include "Source/Game/Game.h"
#include "Source/Game/Shader.h"
#include "Source/Game/Texture.h"
#include "Source/Game/Mesh.h"
#include "Source/Game/MeshComponent.h"
#include "Source/Math/mymath.h"
#include "Source/Game/CameraComponent.h"
#include "Source/Game/Component.h"
#include "Source/Game/AudioSystem.h"
#include "Source/Game/Scene.h"

using namespace DirectX;

Direct3D::Direct3D()
	: m_viewPort({})
	, m_cmb({})
	, m_meshShader(nullptr)
	, m_matrixBuffer(nullptr)
	, m_lightBuffer(nullptr)
	, m_view(XMMatrixIdentity())
	, m_proj(XMMatrixIdentity())
	, m_bloomPass(nullptr)
	, m_gaussPass1(nullptr)
	, m_gaussPass2(nullptr)
	, m_hWnd(nullptr)
	, m_dirLight()
	, m_cfb()
	, m_clb()
{}

bool Direct3D::Initialize(HWND hWnd, int width, int height)
{
	m_hWnd = hWnd;
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//�f�o�C�X/�X���b�v�`�F�C���쐬
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,	// Direct3D 11.1  ShaderModel 5
		D3D_FEATURE_LEVEL_11_0,	// Direct3D 11    ShaderModel 5
	};

	D3D_FEATURE_LEVEL featureLevel;
	DXGI_SWAP_CHAIN_DESC scDesc = {};
	scDesc.BufferDesc.Width = width;
	scDesc.BufferDesc.Height = height;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�t�@�̌`��
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.BufferDesc.RefreshRate.Numerator = 0;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;
	scDesc.SampleDesc.Count = 1;							// MSAA�͎g�p���Ȃ�
	scDesc.SampleDesc.Quality = 0;							// MSAA�͎g�p���Ȃ�
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�t�@�̎g�p���@
	scDesc.BufferCount = 2;									// �o�b�t�@�̐�
	scDesc.OutputWindow = hWnd;
	scDesc.Windowed = TRUE;									// �E�B���h�E���[�h
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&scDesc,
		&m_swapChain,
		&m_device,
		&featureLevel,
		&m_deviceContext)))
	{
		DEBUG_LOG("Failed to Create DeviceAndSwapChain\n");
		return false;
	}

	//�r���[�|�[�g�̐ݒ�
	m_viewPort = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
	m_deviceContext->RSSetViewports(1, &m_viewPort);

	//���X�^���C�U�̐ݒ�
	ComPtr<ID3D11RasterizerState> rasterizerState;
	D3D11_RASTERIZER_DESC ras = {};
	ras.FillMode = D3D11_FILL_SOLID;
	ras.CullMode = D3D11_CULL_BACK;
	if (FAILED(m_device->CreateRasterizerState(&ras, &rasterizerState)))
	{
		DEBUG_LOG("Failed to CreateRasterrizerState\n");
	}
	m_deviceContext->RSSetState(rasterizerState.Get());

	//�[�x/�X�e���V���o�b�t�@���쐬
	D3D11_TEXTURE2D_DESC txDesc;
	ZeroMemory(&txDesc, sizeof(txDesc));
	txDesc.Width = width;
	txDesc.Height = height;
	txDesc.MipLevels = 1;
	txDesc.ArraySize = 1;
	txDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	txDesc.SampleDesc.Count = 1;
	txDesc.SampleDesc.Quality = 0;
	txDesc.Usage = D3D11_USAGE_DEFAULT;
	txDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	txDesc.CPUAccessFlags = 0;
	txDesc.MiscFlags = 0;
	if (FAILED(m_device->CreateTexture2D(&txDesc, NULL, &m_depthStencilTex))) {
		DEBUG_LOG("Failed to CrateTex2D\n");
		return false;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.Format = txDesc.Format;
	dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsDesc.Texture2D.MipSlice = 0;
	if (FAILED(m_device->CreateDepthStencilView(m_depthStencilTex.Get(), &dsDesc, &m_depthStencilView))) {
		DEBUG_LOG("Failed to CreateDepthStencilView\n");
		return false;
	}

	//�I�t�X�N���[���p�e�N�X�`���쐬
	txDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	txDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	if (FAILED(m_device->CreateTexture2D(&txDesc, NULL, &m_renderTexture))) {
		DEBUG_LOG("Failed to CreateOffScTex\n");
	}
	//SRV�쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = txDesc.MipLevels;
	D3D.GetDevice()->CreateShaderResourceView(m_renderTexture.Get(), &srvDesc, &m_textureSRV);
	//�I�t�X�N���[���pRTV�쐬
	if (FAILED(D3D.GetDevice()->CreateRenderTargetView(m_renderTexture.Get(), NULL, &m_textureRTV))) {
		DEBUG_LOG("Failed to CreateOffScRTV\n");
	}

	//�I�t�X�N���[���e�N�X�`���p���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC vbDesc;
	vbDesc.ByteWidth = sizeof(VERTS_RECT);
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vrData;
	vrData.pSysMem = &VERTS_RECT;
	vrData.SysMemPitch = 0;
	vrData.SysMemSlicePitch = 0;
	if (FAILED(m_device->CreateBuffer(&vbDesc, &vrData, &m_renderTexVB))) {
		DEBUG_LOG("Failed to Create renderTexVB\n");
		return false;
	}

	//�f�t�H���gRTV���쐬
	ComPtr<ID3D11Texture2D> backBuffer;
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(m_device->CreateRenderTargetView(backBuffer.Get(), NULL, &m_defRTV)))
	{
		DEBUG_LOG("Failed to CreateRTV\n");
		return false;
	}

	//�u�����h�X�e�[�g�쐬
	D3D11_BLEND_DESC bDesc;
	bDesc.AlphaToCoverageEnable = TRUE;
	bDesc.IndependentBlendEnable = FALSE;
	bDesc.RenderTarget[0].BlendEnable = TRUE;
	bDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_device->CreateBlendState(&bDesc, &m_blendState);

	//�s��o�b�t�@�쐬
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(ConstantMatrixBuffer);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;
	if (FAILED(m_device->CreateBuffer(&cbDesc, NULL, &m_matrixBuffer))) {
		DEBUG_LOG("Failed to Create MatrixBuffer\n");
		return false;
	}
	//���C�e�B���O�o�b�t�@�쐬
	cbDesc.ByteWidth = sizeof(ConstantLightBuffer);
	if (FAILED(m_device->CreateBuffer(&cbDesc, NULL, &m_lightBuffer))) {
		DEBUG_LOG("Failed to Create LightBuffer\n");
		return false;
	}
	//�|�X�g�G�t�F�N�g�o�b�t�@�쐬
	cbDesc.ByteWidth = sizeof(ConstantFBOBuffer);
	if (FAILED(m_device->CreateBuffer(&cbDesc, NULL, &m_fboBuffer))) {
		DEBUG_LOG("Failed to Create FBOBuffer\n");
		return false;
	}
	//�|�X�g�G�t�F�N�g�p�o�b�t�@�f�[�^������
	m_cfb.viewPort.x = m_viewPort.Width;
	m_cfb.viewPort.y = m_viewPort.Height;
	m_cfb.options.x = 3.0f; //offset
	m_cfb.options.y = 2.5f; //intensity
	math::SetGausWeights(m_cfb.gausWeight, 70.0f);

	//�����_�����O�p�X�쐬(�|�X�g�G�t�F�N�g)
	m_gaussPass1 = new RenderPass(L"Shaders/GausseFilter.hlsl", "VS_pass", "PS_pass1");
	m_gaussPass2 = new RenderPass(L"Shaders/GausseFilter.hlsl", "VS_pass", "PS_pass2");
	m_bloomPass = new RenderPass(L"Shaders/BloomShader.hlsl", "VS_pass", "PS_pass");

	//�V�F�[�_�[�ǂݍ���
	if (!LoadShader())
	{
		DEBUG_LOG("Failed to load Shaders\n");
		return false;
	}

	//�T���v���[�X�e�[�g�쐬
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.MipLODBias = 0;
	sampDesc.MaxAnisotropy = 1;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	D3D.GetDevice()->CreateSamplerState(&sampDesc, &m_samplerState);

	//Direct2D��������
	Direct2D::CreateInstance();
	if (!D2D.Initialize(hWnd)) {
		DEBUG_LOG("Failed to Init D2D\n");
		return false;
	}

	DEBUG_LOG("D3D Initialized\n");
	return true;
}

void Direct3D::Shutdown()
{
	m_meshShader->Unload();
	delete m_meshShader;
}

void Direct3D::DrawMeshes()
{
	auto sky = GAME.GetActiveScene()->GetSkyColor();
	float clearColor[4] = {sky.x, sky.y, sky.z, sky.w};

	//�e�N�X�`���֕`��ݒ�
	m_deviceContext->OMSetRenderTargets(1, m_textureRTV.GetAddressOf(), m_depthStencilView.Get());
	m_deviceContext->OMSetBlendState(m_blendState.Get(), NULL, 0xffffffff);
	m_deviceContext->ClearRenderTargetView(m_textureRTV.Get(), clearColor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_meshShader->SetShaders();
	SetLight(m_ambientLight, m_dirLight);
	//�e�I�u�W�F�N�g�`��
	for (auto mc : m_meshComps)
	{
		mc->Draw();
	}
}

void Direct3D::Render()
{
	m_deviceContext->RSSetViewports(1, &m_viewPort);

	//�S�I�u�W�F�N�g�`��
	DrawMeshes();
	
	//�|�X�g�G�t�F�N�g
	m_bloomPass->DrawPass(m_textureSRV.GetAddressOf());
	m_gaussPass1->DrawPass(m_bloomPass->GetSrv());
	m_gaussPass2->SetRTV(m_defRTV); //RT���o�b�N�o�b�t�@�ɐݒ�
	ID3D11ShaderResourceView*const* textures[2] = 
	{m_textureSRV.GetAddressOf(), m_gaussPass1->GetSrv()};
	m_gaussPass2->DrawPass(textures, 2);

	//UI�`��(�o�b�N�o�b�t�@�ɕ`��)
	D2D.Draw();

	m_swapChain->Present(0, 0);
}

bool Direct3D::LoadShader()
{
	if (m_meshShader != nullptr)
		delete m_meshShader;

	m_meshShader = new Shader();
	if (!m_meshShader->Load(
		L"Shaders/PhongVertex.hlsl", 
		L"Shaders/PhongPixel.hlsl", 
		"main",  "main",
		vertLayout::VERTEX3D,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)) 
	{
		return false;
	}

	return true;
}

void Direct3D::SetConstantBuffers() //m_cmb��m_clb���Z�b�g���Ă�����s����
{
	//�R���X�^���g�o�b�t�@���X�V
	m_deviceContext->UpdateSubresource(m_matrixBuffer.Get(), 0, NULL, &m_cmb, 0, 0);
	m_deviceContext->UpdateSubresource(m_lightBuffer.Get(), 0, NULL, &m_clb, 0, 0);
	//�V�F�[�_�[�ɃR���X�^���g�o�b�t�@���Z�b�g
	m_deviceContext->VSSetConstantBuffers(0, 1, m_matrixBuffer.GetAddressOf());
	m_deviceContext->PSSetConstantBuffers(0, 1, m_lightBuffer.GetAddressOf());
}
//m_clb�̃Z�b�g�֐�
void Direct3D::SetLight(const Vector3& ambient, const DirectionalLight& dirLight)
{
	XMStoreFloat4(&m_clb.ambient, ConvertToXMVECTOR(ambient));
	m_clb.dirLight = dirLight;
}
void Direct3D::SetMaterial(const Material& mat)
{
	m_clb.mat = mat;
}
//m_cmb�̃Z�b�g�֐�
void Direct3D::SetWorldMatrix(const XMMATRIX& matrix)
{
	//C++�ň����Q�����z���HLSL�������Q�����z��ł͍s�Ɨ񂪈قȂ邽�ߓ]�u����
	XMStoreFloat4x4(&m_cmb.world, XMMatrixTranspose(matrix));
}
void Direct3D::SetViewMatrix(const XMMATRIX& matrix)
{
	XMStoreFloat4x4(&m_cmb.view, XMMatrixTranspose(matrix));
	m_view = matrix;
	//���C�e�B���O�p�J�����ʒu���o
	XMFLOAT4X4 invView;
	XMStoreFloat4x4(&invView, XMMatrixInverse(nullptr, matrix));
	XMStoreFloat4(&m_clb.eyePos, XMVectorSet(invView._41, invView._42, invView._43, 1.0f));
	//�I�[�f�B�I�̃��X�i�[���X�V
	GAME.GetAudioSystem()->SetListner(m_view);
}
void Direct3D::SetProjMatrix(const XMMATRIX& matrix)
{
	XMStoreFloat4x4(&m_cmb.projection, XMMatrixTranspose(matrix));
	m_proj = matrix;
}

//�|�C���g��̎O�������W���Z�o�iscreenPoint�̓N���C�A���g���W�j
Vector3 Direct3D::Unproject(const POINT& screenPoint, float z) const
{
	//XXX:z=1�̍ۍŉ��������擾���Ȃ�

	XMMATRIX InvProj, InvView, InvVp;

	float w = m_viewPort.Width / 2;
	float h = m_viewPort.Height / 2;
	//�r���[�|�[�g�ϊ��s��
	XMMATRIX vp = XMMatrixSet(w,  0, 0, 0,
							  0, -h, 0, 0,
							  0,  0, 1, 0,
							  w,  h, 0, 1);
	InvVp   = XMMatrixInverse(nullptr, vp);
	InvView = XMMatrixInverse(nullptr, m_view);
	InvProj = XMMatrixInverse(nullptr, m_proj);
	auto unproj = InvVp * InvProj * InvView;

	XMFLOAT3 retVal;
	XMFLOAT3 scVec = { (float)screenPoint.x, (float)screenPoint.y, z };
	auto worldVec = XMVector3Unproject(XMLoadFloat3(&scVec),
		m_viewPort.TopLeftX,
		m_viewPort.TopLeftY,
		m_viewPort.Width,
		m_viewPort.Height,
		m_viewPort.MinDepth,
		m_viewPort.MaxDepth,
		m_proj, m_view, XMMatrixIdentity());
	//�X�N���[�����W��unproj�ŕϊ���w=1�ɂ���
	auto temp = XMVector3TransformCoord(XMLoadFloat3(&scVec), unproj);
	///XMFLOAT3 t;
	XMStoreFloat3(&retVal, temp);
	//XMStoreFloat3(&retVal, worldVec);
	//DEBUG_LOG("unproj : %f, %f, %f  XMunproj : %f, %f, %f\n", t.x, t.y, t.z, retVal.x, retVal.y, retVal.z);
	//���ʂ������̂���m_proj,m_view�ɖ�肪����

	return Vector3(retVal.x, retVal.y, retVal.z);
}

void Direct3D::GetScreenDirection(Vector3& outStart, Vector3& outDir) const
{
	POINT screenPoint = { static_cast<LONG>(D3D.m_viewPort.Width / 2),
						  static_cast<LONG>(D3D.m_viewPort.Height / 2) };
	outStart = Unproject(screenPoint, 0.0f);
	Vector3 end = Unproject(screenPoint, 0.9f);
	outDir = end - outStart;
	outDir.Normalize();
}
Vector3 Direct3D::GetScreenDirection() const
{
	POINT screenPoint = { static_cast<LONG>(D3D.m_viewPort.Width / 2),
						  static_cast<LONG>(D3D.m_viewPort.Height / 2) };
	auto start = Unproject(screenPoint, 0.0f);
	Vector3 end = Unproject(screenPoint, 0.9f);
	auto outDir = end - start;
	outDir.Normalize();
	return outDir;
}

void Direct3D::UnloadData()
{
	for (auto t : m_textures)
	{
		t.second->Unload();
		delete t.second;
	}
	m_textures.clear();

	for (auto m : m_meshes)
	{
		m.second->Unload();
		delete m.second;
	}
	m_meshes.clear();
}

//MeshComponent�̃R���X�^���N�^�A�f�X�g���N�^�̍ۂɎ��s
void Direct3D::AddMeshComp(MeshComponent* meshcomp)
{
	m_meshComps.emplace_back(meshcomp);
}
void Direct3D::RemoveMeshComp(MeshComponent* meshcomp)
{
	auto it = std::find(m_meshComps.begin(), m_meshComps.end(), meshcomp);
	m_meshComps.erase(it);
}

Texture* Direct3D::GetTexture(const std::string& fileName) //Mesh�N���X���ȂǂŎg�p
{
	Texture* tex = nullptr;
	auto iter = m_textures.find(fileName);
	if (iter != m_textures.end())
	{
		tex = iter->second;
	}
	else //������Ȃ���΍쐬���ă��[�h
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			m_textures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

Mesh* Direct3D::GetMesh(const std::string& fileName)
{
	Mesh* m = nullptr;
	auto iter = m_meshes.find(fileName);
	if (iter != m_meshes.end())
	{
		m = iter->second;
	}
	else //������Ȃ���΍쐬���ă��[�h(����:MeshComponent���ʂō쐬����K�v������)
	{
		m = new Mesh();
		if (m->Load(fileName))
		{
			m_meshes.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}
