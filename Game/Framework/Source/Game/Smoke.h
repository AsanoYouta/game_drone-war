#pragma once
#include "ParticleEmitter.h"

class Smoke : public ParticleEmitter
{
public:
	Smoke(const Vector3& pos, float emitTime);
};

