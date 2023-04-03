#pragma once
#include "framework.h"
#include "Source/DirectX/structs.h"
#include "Source/Math/Collision.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool Load(const std::string& filename);
	void Unload();

	class VertexArray* GetVertexArray() const { return m_vertexArray; }
	class Texture* GetTexture(size_t index);
	const std::string& GetShaderName() const { return m_shaderName; }
	AABB GetAABB() { return m_aabb; }
	Sphere GetSphere() const { return m_sphere; }
	Material GetMaterial() const { return m_material; }

private:
	//このメッシュのテクスチャ群
	std::vector<class Texture*> m_textures;
	//このメッシュの頂点配列
	class VertexArray* m_vertexArray;
	//使用するシェーダーの名前
	std::string m_shaderName;

	//鏡面反射係数
	float m_specPow;
	//マテリアル
	Material m_material;
	//境界球の半径
	float m_radius;
	//コリジョン用構造体
	AABB m_aabb;
	Sphere m_sphere;
};

