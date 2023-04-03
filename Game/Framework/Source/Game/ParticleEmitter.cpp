#include "ParticleEmitter.h"
#include "MeshComponent.h"
#include "Source/DirectX/DirectX.h"
#include "Particle.h"
#include "Game.h"

ParticleEmitter::ParticleEmitter(UINT texNum)
	:GameObject(500)
	,m_emitted(false)
	,m_emitTime(0.0f)
{
	SetScale(1.0f);
	m_particleModel = new MeshComponent(this);
	SetTexture(texNum);
	SetOneSync(false);
	m_instances[0]->SetScale(0.0f);

	//疑似乱数
	std::random_device rnd;
	m_mt.seed(rnd());
}

void ParticleEmitter::SetMaterial(Material mat)
{
	m_particleModel->SetMaterial(mat);
}

void ParticleEmitter::SetTexture(UINT texNum)
{
	switch (texNum)
	{
	case 1:
		m_particleModel->SetMesh(D3D.GetMesh("Assets/Models/Particle.gpmesh"));
		break;
	case 2:
		m_particleModel->SetMesh(D3D.GetMesh("Assets/Models/Particle_Hard.gpmesh"));
		break;
	case 3:
		m_particleModel->SetMesh(D3D.GetMesh("Assets/Models/Particle_Smoke.gpmesh"));
		break;
	default:
		m_particleModel->SetMesh(D3D.GetMesh("Assets/Models/Particle.gpmesh"));
		break;
	}
}

void ParticleEmitter::GenerateScatter(
	UINT gNumSq, 
	float force, float lifespan, 
	float mulVec, float addScale,
	XMFLOAT4 addCol)
{
	for (UINT i = 0; i < gNumSq; i++)
	{
		for (UINT j = 0; j < gNumSq; j++)
		{
			float azimuth = (j / float(gNumSq - 1)) * (2.0f * PI);
			float elev = (i / float(gNumSq - 1)) * PI;
			Vector3 velocity = Vector3(sinf(elev) * cosf(azimuth), cosf(elev), sinf(elev) * sin(azimuth)) * force;
			new Particle(this, velocity, lifespan, mulVec, addScale, addCol);
		}
	}
	m_emitted = true;
}

void ParticleEmitter::GenerateRandScatter(
	UINT gNumSq,
	float maxForce, float lifespan,
	float mulVec, float addScale,
	XMFLOAT4 addCol)
{
	std::uniform_int_distribution<> force(0, maxForce);
	for (UINT i = 0; i < gNumSq; i++)
	{
		for (UINT j = 0; j < gNumSq; j++)
		{
			float azimuth = (j / float(gNumSq - 1)) * (2.0f * PI);
			float elev = (i / float(gNumSq - 1)) * PI;
			Vector3 velocity = Vector3(sinf(elev) * cosf(azimuth), cosf(elev), sinf(elev) * sin(azimuth)) * (float)force(m_mt);
			new Particle(this, velocity, lifespan, mulVec, addScale, addCol);
		}
	}
	m_emitted = true;
}

void ParticleEmitter::EmitParticle(float emitTime, UINT gNum, float posRange, float gAccel,
	Vector3 velocity, float velRange, float lifespan,float mulVec, float addScale, XMFLOAT4 col, XMFLOAT4 addCol)
{
	m_emitTime = emitTime;
	m_gNum = gNum;
	m_gAccel = gAccel;
	m_posRange = posRange;
	m_velocity = velocity;
	m_velRange = velRange;
	m_lifespan = lifespan;
	m_mulVec = mulVec;
	m_addScale = addScale;
	m_color = col;
	m_addCol = addCol;
	m_emitted = true;
}

void ParticleEmitter::UpdateGameObject(float deltaTime)
{
	if (m_emitted)
	{
		m_emitTime -= deltaTime;

		if (m_emitTime > 0)
		{
			std::uniform_real_distribution<> dist(-m_posRange, m_posRange);
			std::uniform_real_distribution<> v(-m_velRange, m_velRange);
			for (UINT i = 0; i < m_gNum; i++)
			{
				auto randVel = m_velocity + Vector3((float)v(m_mt), (float)v(m_mt), (float)v(m_mt));
				auto p = new Particle(this, randVel, m_lifespan, m_mulVec, m_addScale, m_addCol);
				auto pos = GetPosition();
				Vector3 gPos = Vector3(pos.x + (float)dist(m_mt), pos.y + (float)dist(m_mt), pos.z + (float)dist(m_mt));
				p->SetPosition(gPos);
				p->SetGravityAccel(m_gAccel);
				p->SetColor(m_color);
			}
		}
		else if(m_instances.size() <= 2)
		{
			SetState(Dead);
		}

		//プレイヤー方向へ回転
		auto toPlayer = Vector3::Normalized(GAME.GetPlayer()->GetPosition() - GetPosition());
		RotateToNewForward(toPlayer);
		//回転を各インスタンスに同期
		for (auto ins : m_instances)
		{
			ins->SetRotation(GetRotation());
		}
	}
}

void ParticleEmitter::Destroy()
{
	SetState(Dead);
}
