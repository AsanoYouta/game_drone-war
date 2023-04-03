#pragma once
#include "UIScreen.h"
#include "InputSystem.h"

class PauseMenu : public UIScreen
{
public:
	PauseMenu();
	~PauseMenu();

	void ProcessInput(const InputState& state) override;
	void Update(float deltaTime) override;

private:
	int m_pauseCount;
};

