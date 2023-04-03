#include "Spark.h"

Spark::Spark(const Vector3& pos)
	:ParticleEmitter()
{
	SetPosition(pos);
	SetMaterial(material::Explode);
	SetScale(0.5f);
	GenerateRandScatter(5, 100.0f, 0.3f, 0.97f, -3.0f, XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
}

Spark::Spark(const Vector3& pos, float scale, 
	UINT gNumSq,
	float force, float lifespan,
	float mulVec, float addScale,
	XMFLOAT4 addCol)
{
	SetPosition(pos);
	SetMaterial(material::EmmisiveWhite);
	SetScale(scale);
	GenerateRandScatter(gNumSq, force, lifespan, mulVec, addScale, addCol);
}
