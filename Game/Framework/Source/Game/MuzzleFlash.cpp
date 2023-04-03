#include "MuzzleFlash.h"
#include "Particle.h"
#include "Source/DirectX/structs.h"

MuzzleFlash::MuzzleFlash(const Vector3& pos)
	:ParticleEmitter()
{
	SetPosition(pos);
	SetMaterial(material::Flash);

	auto emitter = dynamic_cast<ParticleEmitter*>(this);
	auto col = XMFLOAT4(0, 0, 0, 0);
	auto vel = Vector3::Zero;

	new Particle(
		emitter,
		vel, 
		0.05f, 
		1.0f, 
		0.0f,
		col
	);
}