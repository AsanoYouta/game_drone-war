#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Source/Math/mymath.h"
#include <fstream>
#include <sstream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

Mesh::Mesh()
	:m_radius(0)
	,m_vertexArray(nullptr)
	,m_specPow(0.0f)
	,m_aabb(-Vector3::Infinity, Vector3::Infinity)
	,m_sphere(Vector3::Zero, 0.0f)
	,m_material(material::White)
{
}
Mesh::~Mesh()
{}

bool Mesh::Load(const std::string& fileName)

{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		DEBUG_LOG("Mesh file not found : %s\n", fileName.c_str());
		return false;
	}
	
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		DEBUG_LOG("Mesh %s is not valid json\n", fileName.c_str());
		return false;
	}

	int version = doc["version"].GetInt();

	if (version != 1)
	{
		DEBUG_LOG("Mesh %s not version 1\n", fileName.c_str());
	    return false;
	}

	m_shaderName = doc["shader"].GetString();

	m_specPow = static_cast<float>(doc["specularPower"].GetDouble());
	m_material.specular.w = m_specPow;

	//TODO:後に編集
	size_t vertSize = 8; //一つの頂点に格納されているfloatの数

	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		DEBUG_LOG("Mesh %s has no textures, there should be at least one\n", fileName.c_str());
		return false;
	}

	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		std::string texName = textures[i].GetString();
		Texture* t = D3D.GetTexture(texName);
		if (t == nullptr)
		{
			t = D3D.GetTexture("Assets/Textures/Default.png");
			DEBUG_LOG("Failed to Find texture : %s\n", fileName.c_str());
		}
		m_textures.emplace_back(t);
	}

	//頂点群読み込み
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		DEBUG_LOG("Mesh %s has no vertices\n", fileName.c_str());
		return false;
	}

	std::vector<Vertex3D> vertices;
	vertices.reserve(vertsJson.Size());
	m_radius = 0.0f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		//8つの要素があると仮定 頂点座標、法線、UV
		const rapidjson::Value& vertInfo = vertsJson[i];
		if (!vertInfo.IsArray() || vertInfo.Size() != 8)
		{
			DEBUG_LOG("Unexpected vertex format for %s\n", fileName.c_str());
			return false;
		}

		Vector3 pos(
			static_cast<float>(vertInfo[0].GetDouble()), 
			static_cast<float>(vertInfo[1].GetDouble()), 
			static_cast<float>(vertInfo[2].GetDouble())
		);
		m_radius = std::fmax(m_radius, pos.LengthSq()); //posは原点からの距離
		m_aabb.UpdateMaxMin(pos);

		Vertex3D v = {
			XMFLOAT3(pos.x, pos.y, pos.z),
			XMFLOAT3(static_cast<float>(vertInfo[3].GetDouble()), static_cast<float>(vertInfo[4].GetDouble()), static_cast<float>(vertInfo[5].GetDouble())),
			XMFLOAT2(static_cast<float>(vertInfo[6].GetDouble()), static_cast<float>(vertInfo[7].GetDouble()))
		};
		
		vertices.emplace_back(v);
	}
	m_radius = std::sqrt(m_radius);
	m_sphere.m_radius = m_radius;

	//index情報読み込み
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		DEBUG_LOG("Mesh %s has no indices\n", fileName.c_str());
		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& indInfo = indJson[i];
		if (!indInfo.IsArray() || indInfo.Size() != 3)
		{
			DEBUG_LOG("Invalid indices for %s\n", fileName.c_str());
			return false;
		}

		indices.emplace_back(indInfo[0].GetUint());
		indices.emplace_back(indInfo[1].GetUint());
		indices.emplace_back(indInfo[2].GetUint());
	}

	m_vertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()),
									indices.data(), static_cast<unsigned>(indices.size()));

	DEBUG_LOG("Loaded MeshFile : %s\n", fileName.c_str());
	return true;
}

void Mesh::Unload()
{
	delete m_vertexArray;
	m_vertexArray = nullptr;
}

Texture* Mesh::GetTexture(size_t index)
{
	if (index < m_textures.size())
	{
		return m_textures[index];
	}
	else
	{
		return nullptr;
	}
}