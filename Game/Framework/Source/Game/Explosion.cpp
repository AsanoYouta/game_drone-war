#include "Explosion.h"
#include "Source/DirectX/structs.h"
#include "Particle.h"
#include "AudioComponent.h"
#include "Game.h"
#include "FPSObject.h"
#include "FPSCamera.h"

Explosion::Explosion(const Vector3& position)
	:ParticleEmitter()
{
	SetPosition(position);
	SetMaterial(material::Explode);
	SetScale(1.7f);
	GenerateRandScatter(16, 250, 3.0f, 0.97f, -2.7f, XMFLOAT4(-0.05f,-0.46f,-0.46f,0.0f));

	auto audioComp = new AudioComponent(this);
	audioComp->PlayEvent("event:/Explosion3D");

	auto player = dynamic_cast<FPSObject*>(GAME.GetPlayer());
	float wigDis = 400.0f;
	wigDis *= wigDis;
	float dist = Vector3::DistanceSq(player->GetPosition(), this->GetPosition());
	if (player && dist<= wigDis)
	{
		float amount = 700.0f * ((wigDis-dist) / wigDis);
		player->GetFPSCam()->Wiggle(0.0f, 0.97f, amount);
	}
}
