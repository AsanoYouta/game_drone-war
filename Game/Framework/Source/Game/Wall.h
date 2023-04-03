#pragma once
#include "GameObject.h"
#include "Mesh.h"

class Wall : public GameObject
{
public:
	Wall(Vector3 max, Vector3 min, Mesh* mesh = nullptr);

private:
	class MeshComponent* m_meshComp;
	class BoxComponent* m_boxComp;
};

