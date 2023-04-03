#include "Bullet.h"
#include "BallObject.h"
#include "PhysicalMove.h"
#include "MeshComponent.h"
#include "SphereComponent.h"
#include "AudioComponent.h"
#include "Mesh.h"
#include "Source/DirectX/DirectX.h"
#include "EnemyObject.h"
#include "FPSObject.h"
#include "Wall.h"
#include "Spark.h"

//プレイヤーが生成する弾オブジェクト
Bullet::Bullet(GameObject* origin, float mass)
	:GameObject()
	,m_origin(origin)
	,m_lifespan(3.0f)
{
	ComputeWorldTransform();

	auto* mc = new MeshComponent(this);
	Mesh* mesh = D3D.GetMesh("Assets/Models/bullet.gpmesh");
	mc->SetMesh(mesh);
	mc->SetMaterial(material::Bullet);
	SetScale(0.8f);

	m_physicalMove = new PhysicalMove(this, mass);
	m_sphereComp = new SphereComponent(this);
	m_sphereComp->SetObjectSphere(mesh->GetSphere());
}

void Bullet::UpdateGameObject(float deltaTime)
{
	m_lifespan -= deltaTime;
	if (m_lifespan <= 0 ||
		m_physicalMove->GetVelocity().LengthSq() <= 500000.0f)
	{
		SetState(Dead);
	}
}

void Bullet::OnCollision(const CollisionInfo& info)
{
	if (info.GameObj != m_origin)
	{
		//GameObjectの派生クラスであるenemyに変換を試みる
		EnemyObject* enemy = dynamic_cast<EnemyObject*>(info.GameObj);
		if (enemy)
		{
			enemy->TakeDamage(35.0f, m_physicalMove->GetVelocity());
			new Spark(info.collidePoint, 0.53f, 7, 270.0f, 0.7f, 0.98f, -0.7f, 
				XMFLOAT4(-0.8f, -0.8f, 0.0f, 0.0f));
			auto fpsObj = dynamic_cast<FPSObject*>(m_origin);
			fpsObj->HitEvent();
			SetState(Dead);
		}
		else if (dynamic_cast<Wall*>(info.GameObj))
		{
			return;
		}
		m_physicalMove->OnCollision(info);
		new Spark(info.collidePoint);
	}
}

void Bullet::AddStrikeForce(const Vector3& force)
{
	m_physicalMove->AddStrikeForce(force);
}

