#pragma once
#include "GameObject.h"

class Building : public GameObject
{
public:
	Building(UINT modelNum);
	static const UINT typeNum = 9;

private:
	class MeshComponent* m_meshComp;
	class BoxComponent* m_boxComp;
};

