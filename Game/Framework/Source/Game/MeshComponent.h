#pragma once
#include "framework.h"
#include "Source/DirectX/structs.h"
#include "Component.h"

class MeshComponent : public Component
{
public:
	MeshComponent(class GameObject* owner);
	~MeshComponent();

	//�A�N�e�B�u�ȃV�F�[�_�[�ŕ`��
	virtual void Draw();
	//mesh���Z�b�g
	virtual void SetMesh(class Mesh* mesh);
	//�g�p����e�N�X�`�����w��
	void SetTextureIndex(size_t index);
	void SetMaterial(Material mat) { m_material = mat; }

protected:
	class Mesh* m_mesh;
	size_t m_textureIndex;
	Material m_material;
};

