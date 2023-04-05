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
		//�A�N�e�B�u�e�N�X�`���̐ݒ�
		Texture* t = m_mesh->GetTexture(m_textureIndex);
		if (t) { t->SetSrvAndSampler(D3D.GetSamplerState().GetAddressOf()); }
		//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���Z�b�g
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetBuffers(m_owner->GetInstanceBuffer());

		//���[���h�ϊ��s���ݒ�(�r���[�A�v���W�F�N�V�����s��̓A�N�e�B�u�J������Update�֐��ŃZ�b�g�ς�)
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
	//�f�t�H���g�}�e���A������
	m_material = mesh->GetMaterial();
}

void MeshComponent::SetTextureIndex(size_t index)
{
	m_textureIndex = index;
}