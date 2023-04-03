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
	//���̃��b�V���̃e�N�X�`���Q
	std::vector<class Texture*> m_textures;
	//���̃��b�V���̒��_�z��
	class VertexArray* m_vertexArray;
	//�g�p����V�F�[�_�[�̖��O
	std::string m_shaderName;

	//���ʔ��ˌW��
	float m_specPow;
	//�}�e���A��
	Material m_material;
	//���E���̔��a
	float m_radius;
	//�R���W�����p�\����
	AABB m_aabb;
	Sphere m_sphere;
};

