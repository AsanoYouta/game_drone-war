#include "Wall.h"
#include "BoxComponent.h"
#include "MeshComponent.h"

Wall::Wall(Vector3 max, Vector3 min, Mesh* mesh)
{
	SetType(Static);

	m_boxComp = new BoxComponent(this);
	auto aabb = AABB(max, min);
	m_boxComp->SetObjectBox(aabb);

	if (mesh)
	{
		m_meshComp = new MeshComponent(this);
		m_meshComp->SetMesh(mesh);
	}
}