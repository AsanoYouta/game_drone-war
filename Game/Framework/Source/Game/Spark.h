#pragma once
#include "ParticleEmitter.h"

class Spark : public ParticleEmitter
{
public:
	Spark(const Vector3& pos);
	Spark(const Vector3& pos, float scale, UINT gNumSq,
		float force, float lifespan,
		float mulVec, float addScale,
		XMFLOAT4 addColl);
};

