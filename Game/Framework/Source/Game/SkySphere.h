#pragma once
#include "GameObject.h"

class SkySphere : public GameObject
{
public:
	SkySphere(UINT texNum);

private:
	class MeshComponent* m_meshComp;
};

