#pragma once
#include "Scene.h"

class BattleScene : public Scene
{
public:
	BattleScene(const std::string& name);

	void Load() override;
	void Unload() override;
	void ProcessInput(const InputState& state) override;
};

