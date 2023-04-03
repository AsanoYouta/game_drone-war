#include "Building.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "BoxComponent.h"

Building::Building(UINT num)
{
	SetType(Static);

	m_meshComp = new MeshComponent(this);
	Mesh* mesh;
	//�Ⴓ��
	switch (num)
	{
	case 1:
		mesh = D3D.GetMesh("Assets/Models/Shop2.gpmesh");
		m_meshComp->SetMesh(mesh);
		m_meshComp->SetMaterial(material::RoughWhite);
		break;
	case 2:
		mesh = D3D.GetMesh("Assets/Models/Industrial2.gpmesh");
		m_meshComp->SetMesh(mesh);
		break;
	case 3:
		mesh = D3D.GetMesh("Assets/Models/Industrial.gpmesh");
		m_meshComp->SetMesh(mesh);
		m_meshComp->SetMaterial(material::RoughWhite);
		break;
	case 4:
		mesh = D3D.GetMesh("Assets/Models/Apart.gpmesh");
		m_meshComp->SetMesh(mesh);
		m_meshComp->SetMaterial(material::RoughWhite);
		break;
	case 5:
		mesh = D3D.GetMesh("Assets/Models/Bill3.gpmesh");
		m_meshComp->SetMesh(mesh);
		m_meshComp->SetMaterial(material::RoughWhite);
		break;
	case 6:
		mesh = D3D.GetMesh("Assets/Models/Bill2.gpmesh");
		m_meshComp->SetMesh(mesh);
		m_meshComp->SetMaterial(material::RoughWhite);
		break;
	case 7:
		mesh = D3D.GetMesh("Assets/Models/Bill1.gpmesh");
		m_meshComp->SetMesh(mesh);
		break;
	case 8:
		mesh = D3D.GetMesh("Assets/Models/Tower1.gpmesh");
		m_meshComp->SetMesh(mesh);
		break;
	case 9:
		mesh = D3D.GetMesh("Assets/Models/Tower2.gpmesh");
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
