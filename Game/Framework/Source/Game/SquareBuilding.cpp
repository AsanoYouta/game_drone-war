#include "SquareBuilding.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "BoxComponent.h"

SquareBuilding::SquareBuilding(UINT num)
{
	SetType(Static);

	m_meshComp = new MeshComponent(this);
	Mesh* mesh;
	//’á‚³‡
	switch (num)
	{
	case 1:
		mesh = D3D.GetMesh("Assets/Models/Shop2.gpmesh");
		m_meshComp->SetMesh(mesh);
		SetScale(0.7f);
		break;
	case 2:
		mesh = D3D.GetMesh("Assets/Models/Shop1.gpmesh");
		m_meshComp->SetMesh(mesh);
		SetScale(0.7f);
		break;
	case 3:
		mesh = D3D.GetMesh("Assets/Models/Bill3.gpmesh");
		m_meshComp->SetMesh(mesh);
		m_meshComp->SetMaterial(material::RoughWhite);
		break;
	case 4:
		mesh = D3D.GetMesh("Assets/Models/Bill2.gpmesh");
		m_meshComp->SetMesh(mesh);
		m_meshComp->SetMaterial(material::RoughWhite);
		break;
	case 5:
		mesh = D3D.GetMesh("Assets/Models/Bill1.gpmesh");
		m_meshComp->SetMesh(mesh);
		break;
	case 6:
		mesh = D3D.GetMesh("Assets/Models/Tower1.gpmesh");
		m_meshComp->SetMesh(mesh);
		break;
	case 7:
		mesh = D3D.GetMesh("Assets/Models/Tower2.gpmesh");
		m_meshComp->SetMesh(mesh);
		m_meshComp->SetMaterial(material::RoughWhite);
		break;

	default:
		mesh = D3D.GetMesh("Assets/Models/Shop2.gpmesh");
		m_meshComp->SetMesh(mesh);
		SetScale(0.7f);
		break;
	}

	m_boxComp = new BoxComponent(this);
	m_boxComp->SetObjectBox(mesh->GetAABB());
}
