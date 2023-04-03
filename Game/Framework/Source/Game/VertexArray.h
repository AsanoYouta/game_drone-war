#pragma once
#include "framework.h"
#include "Source/DirectX/DirectX.h"

class VertexArray
{
public:
	VertexArray(const Vertex3D* verts, unsigned int numVerts,
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	void SetBuffers();
	void SetBuffers(ID3D11Buffer* instanceBuffer);
	unsigned int GetNumIndices() const { return m_numIndices; }
	unsigned int GetNumVerts() const { return m_numVerts; }

private:
	unsigned int m_numVerts;
	unsigned int m_numIndices;

	//���_�o�b�t�@
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D11Buffer> m_indexBuffer;

	bool CreateVertexBuffer(const Vertex3D* verts, unsigned int numVerts);
	bool CreateIndexBuffer(const unsigned int* indices, unsigned int numIndices);
};

