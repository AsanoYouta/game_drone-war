#pragma once
#include "GameObject.h"

class Floor : public GameObject
{
public:
	//1:茶色金属　2:アスファルト
	Floor(UINT texNum);

private:
	class MeshComponent* m_meshComp;
	class BoxComponent* m_boxComp;
};

