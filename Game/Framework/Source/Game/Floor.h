#pragma once
#include "GameObject.h"

class Floor : public GameObject
{
public:
	//1:���F�����@2:�A�X�t�@���g
	Floor(UINT texNum);

private:
	class MeshComponent* m_meshComp;
	class BoxComponent* m_boxComp;
};

