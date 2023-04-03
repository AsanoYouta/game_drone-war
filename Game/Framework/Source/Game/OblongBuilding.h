#pragma once
#include "GameObject.h"

class OblongBuilding : public GameObject
{
public:
	OblongBuilding(UINT num);

	static const UINT typeNum = 2;

private:
	class MeshComponent* m_meshComp;
	class BoxComponent* m_boxComp;
};

