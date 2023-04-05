#include "MeshComponent.h"
#include "VertexArray.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"

MeshComponent::MeshComponent(GameObject* owner)
	:Component(owner)
	, m_mesh(nullptr)
	, m_textureIndex(0)
	, m_material(material::White)
{
	D3D.AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	D3D.RemoveMeshComp(this);
}

void MeshComponent::Draw()
{
	if (m_mesh)
	{
		D3D.SetMaterial(m_material);
		//アクティブテクスチャの設定
		Texture* t = m_mesh->GetTexture(m_textureIndex);
		if (t) { t->SetSrvAndSampler(D3D.GetSamplerState().GetAddressOf()); }
		//頂点バッファとインデックスバッファをセット
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetBuffers(m_owner->GetInstanceBuffer());

		//ワールド変換行列を設定(ビュー、プロジェクション行列はアクティブカメラのUpdate関数でセット済み)
		D3D.SetWorldMatrix(m_owner->GetWorldTransform());
		D3D.SetConstantBuffers();
		
		//D3D.GetDeviceContext()->DrawIndexed(va->GetNumIndices(), 0, 0);
		D3D.GetDeviceContext()->DrawIndexedInstanced(
			va->GetNumIndices(),
			(UINT)m_owner->GetInstances().size(),
			0, 0, 0);
	}
}

void MeshComponent::SetMesh(Mesh* mesh)
{
	m_mesh = mesh;
	//デフォルトマテリアル割当
	m_material = mesh->GetMaterial();
}

void MeshComponent::SetTextureIndex(size_t index)
{
	m_textureIndex = index;
}