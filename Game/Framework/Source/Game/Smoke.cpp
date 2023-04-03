#include "Smoke.h"

Smoke::Smoke(const Vector3& pos, float emitTime)
	:ParticleEmitter(3)
{
	SetPosition(pos);
	SetMaterial(material::Smoke);
	SetScale(0.5f);
	EmitParticle(
		emitTime, 
		5,
		0.1f,
		-150.0f, 
		Vector3::Up*10.0f, 
		50.0f, 
		2.0f,
		1.0f, 
		2.5f, 
		XMFLOAT4(0.2f, 0.2f, 0.2f, 0.6f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, -0.0f));
}