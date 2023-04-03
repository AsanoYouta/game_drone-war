#include "BallObject.h"
#include "PhysicalMove.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Source/DirectX/DirectX.h"

BallObject::BallObject()
	:GameObject()
{
	auto* mc = new MeshComponent(this);
	Mesh* mesh = D3D.GetMesh("Assets/Models/Sphere.gpmesh");
	mc->SetMesh(mesh);

	m_physicalMove = new PhysicalMove(this);
}

void BallObject::UpdateGameObject(float deltaTime)
{
	//DEBUG_LOG("ballPos:%f, %f, %f\n", GetPosition());
}

void BallObject::AddStrikeForce(const Vector3& force)
{
	m_physicalMove->AddStrikeForce(force);
}
