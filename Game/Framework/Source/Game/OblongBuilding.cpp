#include "OblongBuilding.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "BoxComponent.h"

OblongBuilding::OblongBuilding(UINT num)
{
	SetType(Static);

	m_meshComp = new MeshComponent(this);
	Mesh* mesh;
	switch (num)
	{
	case 1:
		mesh = D3D.GetMesh("Assets/Models/Industrial.gpmesh");
		m_meshComp->SetMesh(mesh);
		m_meshComp->SetMaterial(material::RoughWhite);
		break;
	case 2:
		mesh = D3D.GetMesh("Assets/Models/Apart.gpmesh");
		m_meshComp->SetMesh(mesh);
		m_meshComp->SetMaterial(material::RoughWhite);
		break;
	default:
		mesh = D3D.GetMesh("Assets/Models/Industrial.gpmesh");
		m_meshComp->SetMesh(mesh);
		m_meshComp->SetMaterial(material::RoughWhite);
		break;
	}

	m_boxComp = new BoxComponent(this);
	m_boxComp->SetObjectBox(mesh->GetAABB());
}
