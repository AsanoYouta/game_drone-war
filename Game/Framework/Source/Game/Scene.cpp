#include "Scene.h"
#include "TitleScene.h"
#include "BattleScene.h"
#include "Game.h"
#include "GameObject.h"
#include "PauseMenu.h"
#include "SquareBuilding.h"
#include "Building.h"

std::unordered_map<std::string, Scene*> Scene::s_scenes = {};

void Scene::Initialize()
{
	new TitleScene("Title");
	new BattleScene("Battle");
}

Scene* Scene::GetScene(const std::string& name)
{
	return s_scenes.at(name);
}

Scene::Scene(const std::string& name)
	:m_name(name)
{
	s_scenes.insert(std::pair<std::string, Scene*>(name, this));
	m_skyColor = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	std::random_device rnd; //èâä˙ÉVÅ[Éh
	m_rnd.seed(rnd());
}

Scene::~Scene()
{
	DEBUG_LOG("delete Scene : %s\n", m_name.c_str());
}

void Scene::Load()
{
}

void Scene::Unload()
{
}

void Scene::ProcessInput(const InputState& state)
{
}

void Scene::Update(float deltaTime)
{
}

void Scene::PlaceRandBuilding(float blockSize, const Vector3& center, UINT heightMax, UINT heightMin, UINT xNum, UINT zNum)
{
	if (heightMax < heightMin ||
		heightMax > Building::typeNum || 
		heightMin < 1) {
		DEBUG_LOG("PlaceRandBuilding:heightLevel out of range\n");
	}
	std::uniform_int_distribution<> level(heightMin, heightMax);
	GameObject* obj;
	float objSize = blockSize / 3.0f;
	float startX = center.x - objSize * 0.5f;
	float startZ = center.z - objSize * 0.5f;
	bool OblongPlaced = false;
	for (UINT i = 0; i < xNum; i++)
	{
		for (UINT j = 0; j < zNum; j++)
		{
			auto num = level(m_rnd);
			if (OblongPlaced && num == 3 || num == 4)
			{
				while(num == 3 || num == 4) num = level(m_rnd);
			}
			obj = new Building(num);
			obj->SetPosition(Vector3(
				startX + objSize * i,
				center.y,
				startZ + objSize * j));
			//â°í∑Ç¢åöï®ÇæÇ¡ÇΩèÍçá
			if (num == 3 || num == 4) {
				OblongPlaced = true;
				i++;
			}
		}
	}
}

void Scene::PlaceRandSquare(float blockSize, const Vector3& center, UINT heightMax, UINT heightMin, UINT xNum, UINT zNum)
{
	if (heightMax < heightMin ||
		heightMax > SquareBuilding::typeNum || 
		heightMin < 1) {
		DEBUG_LOG("PlaceRandSquare:heightLevel out of range\n");
	}
	std::uniform_int_distribution<> level(heightMin, heightMax);
	GameObject* obj;
	float objSize = blockSize / 3.0f;
	float startX = center.x - objSize*0.5f;
	float startZ = center.z - objSize*0.5f;
	for (UINT i = 0; i < xNum; i++)
	{
		for (UINT j = 0; j < zNum; j ++)
		{
			obj = new SquareBuilding(level(m_rnd));
			obj->SetPosition(Vector3(
				startX + objSize * i,
				center.y,
				startZ + objSize * j));
		}
	}
}
