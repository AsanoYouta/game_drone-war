#pragma once
#include "UIScreen.h"

class Notification : public UIScreen
{
public:
	Notification(std::wstring text, float displayTime, Vector2 pos = Vector2(0.5f, 0.4f));

	void Update(float deltaTime) override;
	void Draw() override;

private:
	float m_elapsed;
	float m_displayTime;
	std::unique_ptr<Text> m_text;
};

