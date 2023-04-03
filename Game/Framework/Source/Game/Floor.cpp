#include "Floor.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "BoxComponent.h"

Floor::Floor(UINT texNum)
{
	SetType(Static);

	m_meshComp = new MeshComponent(this);
	Mesh* mesh = nullptr;
	switch (texNum)
	{
	case 1:
		mesh = D3D.GetMesh("Assets/Models/Floor1.gpmesh");
		break;
	case 2:
		mesh = D3D.GetMesh("Assets/Models/Floor2.gpmesh");
		break;
	case 3:
		mesh = D3D.GetMesh("Assets/Models/Floor3.gpmesh");
		break;
	default:
		mesh = D3D.GetMesh("Assets/Models/Floor1.gpmesh");
		break;
	}

	m_meshComp->SetMesh(mesh);

	m_boxComp = new BoxComponent(this);
	//平面モデルのため、自動作成したサイズに厚みを持たせる
	auto box = mesh->GetAABB();
	box.m_min = box.m_min - Vector3::Up * 2.0f;
	m_boxComp->SetObjectBox(box);
}
