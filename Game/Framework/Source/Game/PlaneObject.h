#pragma once
#include "GameObject.h"

class PlaneObject : public GameObject
{
public:
	PlaneObject();
	~PlaneObject();
	
	class BoxComponent* GetBoxComp() { return m_box; }

private:
	class BoxComponent* m_box;
};

