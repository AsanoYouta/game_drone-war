#pragma once
#include "Scene.h"
#include "SoundEvent.h"

class TitleScene : public Scene
{
public:
	TitleScene(std::string name);

	void Load() override;
	void Unload() override;
	void Update(float deltaTime) override;
	void PlaceFormationEnemy(const Vector3& pos);

private:
	class CameraObject* m_camObj;
	std::vector<class EnemyObject*> m_enemies;
	SoundEvent m_bgm;
};

