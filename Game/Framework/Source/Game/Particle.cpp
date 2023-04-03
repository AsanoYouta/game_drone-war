#include "Particle.h"
#include "GameObject.h"

//形状はParticleEmitterに依存
Particle::Particle(GameObject* owner, 
	Vector3& velocity,
	float lifespan,
	float mulVec,
	float addScale,
	XMFLOAT4 perAddCol)
	:ObjectInstance(owner)
	,m_gAccel(250.0f)
	,m_velocity(velocity)
	,m_lifespan(lifespan)
	,m_mulVec(mulVec)
	,m_addScale(addScale)
	,m_addCol(perAddCol)
{
}

void Particle::Update(float deltaTime)
{
	m_lifespan -= deltaTime;
	if (m_lifespan <= 0)
	{
		SetLive(false);
	}

	//パラメータ更新
	m_velocity *= m_mulVec;
	Vector3 gAccel = -Vector3::Up * m_gAccel;
	m_velocity += gAccel * deltaTime;
	Vector3 newPos = GetPosition() + m_velocity * deltaTime;
	SetPosition(newPos);

	float newScale = GetScale() + m_addScale * deltaTime;
	if (newScale <= 0) newScale = 0;
	SetScale(newScale);

	auto col = GetColor();
	float x = m_addCol.x * deltaTime;
	float y = m_addCol.y * deltaTime;
	float z = m_addCol.z * deltaTime;
	float w = m_addCol.w * deltaTime;
	XMFLOAT4 newCol = XMFLOAT4(col.x + x, col.y + y, col.z + z, col.w + w);
	SetColor(newCol);
}
