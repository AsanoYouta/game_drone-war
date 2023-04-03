#include "PlaneObject.h"
#include "MeshComponent.h"
#include "Source/DirectX/DirectX.h"
#include "Mesh.h"
#include "BoxComponent.h"
#include "Game.h"

PlaneObject::PlaneObject() 
	:GameObject()
{
	SetType(Static);

	SetScale(1.0f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = D3D.GetMesh("Assets/Models/Plane.gpmesh");
	mc->SetMesh(mesh);
	
	m_box = new BoxComponent(this);
	m_box->SetObjectBox(mesh->GetAABB());
}

PlaneObject::~PlaneObject()
{

}
