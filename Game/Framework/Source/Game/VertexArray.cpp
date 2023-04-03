#include "VertexArray.h"

//DirectXに頂点や法線などのデータを渡す
VertexArray::VertexArray(const Vertex3D* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	: m_numVerts(numVerts)
	, m_numIndices(numIndices)
	, m_indexBuffer(nullptr)
	, m_vertexBuffer(nullptr)
{
	if (!CreateVertexBuffer(verts, numVerts)) {
		DEBUG_LOG("Failed CreateVertexBuffer");
	}

	if (!CreateIndexBuffer(indices, numIndices)) {
		DEBUG_LOG("Failed CreateIndexBuffer");
	}
}

VertexArray::~VertexArray()
{
	m_indexBuffer.Reset();
	m_vertexBuffer.Reset();
}

void VertexArray::SetBuffers()
{ 
	//頂点バッファ設定
	UINT strides = sizeof(Vertex3D);
	UINT offsets = 0;
	D3D.GetDeviceContext()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &strides, &offsets);
	//インデックスバッファ設定
	D3D.GetDeviceContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void VertexArray::SetBuffers(ID3D11Buffer* instanceBuffer)
{
	UINT strides[2] = { sizeof(Vertex3D) , sizeof(InstanceData) };
	UINT offsets[2] = { 0, 0 };
	ID3D11Buffer* buffer[2] = { m_vertexBuffer.Get(), instanceBuffer };
	D3D.GetDeviceContext()->IASetVertexBuffers(0, 2, buffer, strides, offsets);
	//インデックスバッファ設定
	D3D.GetDeviceContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

bool VertexArray::CreateVertexBuffer(const Vertex3D* verts, unsigned int numVerts)
{
	D3D11_BUFFER_DESC vbDesc;
	vbDesc.ByteWidth = sizeof(Vertex3D) * numVerts;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vrData;
	vrData.pSysMem = verts;
	vrData.SysMemPitch = 0;
	vrData.SysMemSlicePitch = 0;
	if (FAILED(D3D.GetDevice()->CreateBuffer(&vbDesc, &vrData, &m_vertexBuffer))) {
		return false;
	}

	return true;
}

bool VertexArray::CreateIndexBuffer(const unsigned int* indices, unsigned int numIndices)
{
	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth = sizeof(unsigned int) * numIndices;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA irData;
	irData.pSysMem = indices;
	irData.SysMemPitch = 0;
	irData.SysMemSlicePitch = 0;
	if (FAILED(D3D.GetDevice()->CreateBuffer(&ibDesc, &irData, &m_indexBuffer))) {
		return false;
	}

	return true;
}
