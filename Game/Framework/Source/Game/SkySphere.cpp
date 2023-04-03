#include "SkySphere.h"
#include "MeshComponent.h"
#include "Mesh.h"

SkySphere::SkySphere(UINT texNum)
{
	m_meshComp = new MeshComponent(this);
	Mesh* mesh;
	switch (texNum)
	{
	case 1:
		mesh = D3D.GetMesh("Assets/Models/SkySphere1.gpmesh");
		break;
	case 2:
		mesh = D3D.GetMesh("Assets/Models/SkySphere2.gpmesh");
		break;
	case 3:
		mesh = D3D.GetMesh("Assets/Models/SkySphere3.gpmesh");
		break;
	case 4:
		mesh = D3D.GetMesh("Assets/Models/SkySphere4.gpmesh");
		break;
	default:
		mesh = D3D.GetMesh("Assets/Models/SkySphere1.gpmesh");
		break;
	}
	m_meshComp->SetMesh(mesh);

	SetScale(100.0f);

	m_meshComp->SetMaterial(material::Skybox);
}
