#pragma once
#include "UIScreen.h"

class ClearStage : public UIScreen
{
public:
	ClearStage(float resultTime);

	void Draw() override;

private:
	std::unique_ptr<Text> m_resultTime;
};

