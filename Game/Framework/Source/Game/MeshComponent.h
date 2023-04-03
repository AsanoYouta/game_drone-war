#pragma once
#include "framework.h"
#include "Source/DirectX/structs.h"
#include "Component.h"

class MeshComponent : public Component
{
public:
	MeshComponent(class GameObject* owner);
	~MeshComponent();

	//アクティブなシェーダーで描画
	virtual void Draw();
	//meshをセット
	virtual void SetMesh(class Mesh* mesh);
	//使用するテクスチャを指定
	void SetTextureIndex(size_t index);
	void SetMaterial(Material mat) { m_material = mat; }

protected:
	class Mesh* m_mesh;
	size_t m_textureIndex;
	Material m_material;
};

