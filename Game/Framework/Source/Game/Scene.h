#pragma once
#include "framework.h"
#include <unordered_map>
#include "InputSystem.h"
#include "Source/DirectX/DirectX.h"
#include <random>

class Scene
{
public:
	Scene(const std::string& name);
	~Scene();

	virtual void Load();
	virtual void Unload();
	virtual void ProcessInput(const InputState& state);
	virtual void Update(float deltaTime);
	void PlaceRandBuilding(float blockSize, const Vector3& blockCenter, 
		UINT heightMax, UINT heightMin, UINT xNum = 2, UINT zNum = 2);
	void PlaceRandSquare(float blockSize, const Vector3& blockCenter,
		UINT heightMax, UINT heightMin, UINT xNum = 2, UINT zNum = 2);

	const std::string& GetName() const { return m_name; }
	XMFLOAT4 GetSkyColor() { return m_skyColor; }

	static void Initialize();
	static Scene* GetScene(const std::string& name);

protected:
	std::string m_name;
	XMFLOAT4 m_skyColor;
	std::mt19937 m_rnd;

private:
	static std::unordered_map<std::string, Scene*> s_scenes;
};

