#pragma once

#include "DirectX.h"
#include "RenderPass.h"	
#include "framework.h"
#include "Source/Math/mymath.h"
#include "structs.h"

class Direct3D
{
public:
	D3D11_VIEWPORT m_viewPort;

	//初期化
	bool Initialize(HWND hWnd, int width, int height);
	void Shutdown();
	//描画処理
	void Render();
	//全テクスチャ、メッシュを開放
	void UnloadData();

	HWND GetHWND() const { return m_hWnd; }
	ComPtr<ID3D11Device> GetDevice() const { return m_device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() const { return m_deviceContext; }
	ComPtr<IDXGISwapChain> GetSwapChain() const { return m_swapChain; }
	ComPtr<ID3D11RenderTargetView> GetBackBufferView() const { return m_defRTV; }
	ComPtr<ID3D11SamplerState> GetSamplerState() const { return m_samplerState; }
	void SetWorldMatrix(const XMMATRIX& matrix);
	void SetViewMatrix(const XMMATRIX& matrix);
	void SetProjMatrix(const XMMATRIX& matrix);
	void SetLight(const Vector3& ambient, const DirectionalLight& dirLight);
	void SetMaterial(const Material& mat);
	void SetConstantBuffers();	//注意:すべての変換行列をセットした後に呼ぶ

	void SetAmbientLight(const Vector3& ambient) { m_ambientLight = ambient; }
	void SetDirectionalLight(const DirectionalLight& dirLight) { m_dirLight = dirLight; }
	DirectionalLight& GetDirectionalLight() { return m_dirLight; }

	void AddMeshComp(class MeshComponent* meshcomp);
	void RemoveMeshComp(class MeshComponent* meshcomp);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);

	//逆射影変換
	Vector3 Unproject(const POINT& screenPoint, float z) const;
	void GetScreenDirection(Vector3& outStart, Vector3& outDir) const;
	Vector3 GetScreenDirection() const;

private:
	friend class Direct2D;
	friend class RenderPass;

	HWND m_hWnd;
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11BlendState> m_blendState;

	ComPtr<ID3D11Texture2D> m_depthStencilTex;
	ComPtr<ID3D11RenderTargetView> m_defRTV;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	//オフスクリーンレンダリング用
	ComPtr<ID3D11Texture2D> m_renderTexture;
	ComPtr<ID3D11RenderTargetView> m_textureRTV;
	ComPtr<ID3D11ShaderResourceView> m_textureSRV;
	//ポストエフェクト用
	ComPtr<ID3D11Buffer> m_renderTexVB;
	RenderPass* m_gaussPass1;
	RenderPass* m_gaussPass2;
	RenderPass* m_bloomPass;

	//アセット格納配列
	std::unordered_map<std::string, class Texture*> m_textures;
	std::unordered_map<std::string, class Mesh*> m_meshes;
	std::vector<class MeshComponent*> m_meshComps;

	//シェーダー
	class Shader* m_meshShader;
	//サンプラーステート
	ComPtr<ID3D11SamplerState> m_samplerState;

	//コンスタントバッファ
	ComPtr<ID3D11Buffer> m_matrixBuffer;
	ComPtr<ID3D11Buffer> m_lightBuffer;
	ComPtr<ID3D11Buffer> m_fboBuffer;
	ConstantMatrixBuffer m_cmb;
	ConstantLightBuffer m_clb;
	ConstantFBOBuffer m_cfb;
	XMMATRIX m_proj;
	XMMATRIX m_view;

	//ライティングデータ
	Vector3 m_ambientLight;
	DirectionalLight m_dirLight;

	bool LoadShader();
	void DrawMeshes();

//----------------------------------------------------
//シングルトン
	static inline Direct3D* instance;
	Direct3D();
public:
	static void CreateInstance()
	{
		DeleteInstance();
		instance = new Direct3D();
	}
	static void DeleteInstance()
	{
		if (instance != nullptr) {
			delete instance;
			instance = nullptr;		
		}
	}
	static Direct3D& GetInstance()
	{
		return *instance;
	}
};

#define D3D Direct3D::GetInstance()

