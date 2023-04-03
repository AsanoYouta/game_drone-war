#pragma once
#include "GameObject.h"

class SquareBuilding : public GameObject
{
public:
	SquareBuilding(UINT modelNum);

	static const UINT typeNum = 7;
private:
	class MeshComponent* m_meshComp;
	class BoxComponent* m_boxComp;
};

