#include "FPSObject.h"
#include "Game.h"
#include "MoveComponent2.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "AudioComponent.h"
#include "AudioSystem.h"
#include "FPSCamera.h"
#include "InputSystem.h"
#include "Source/Math/mymath.h"
#include "PhysWorld.h"
#include "Source/Math/Collision.h"
#include "PlaneObject.h"
#include "Bullet.h"
#include "MuzzleFlash.h"
#include "EnemyObject.h"
#include "Explosion.h"
#include "GameOver.h"
#include "HUD.h"

FPSObject::FPSObject()
	:GameObject()
	,m_shoot(false)
	,m_hp(100.0f)
	,m_invincibleTime(0.0f)
	,m_stepCharge(0.0f)
	,m_oneStepSec(3.0f)
	,m_alertDist(150.0f)
	,m_shootingRate(0.05f)
	,m_maxSpread(60.0f)
	,m_remainAmmo(2000)
	,m_lastShot(0.0f)
	,m_accelAmount(400.0f)
	,m_jumpCharge(0.0f)
	,m_hitMarkerDisplay(0.0f)
	,m_charaState(OnGround)
	,m_damagedEnemy(nullptr)
	,m_gameManager(nullptr)
{
	std::random_device rnd; //初期シード
	m_rnd.seed(rnd());

	m_moveComp = new MoveComponent2(this);

	m_audioComp = new AudioComponent(this);
	m_lastFootStep = 0.0f;
	m_footStep = m_audioComp->PlayEvent("event:/Footstep");
	m_footStep.SetPaused(true);
	m_environment = m_audioComp->PlayEvent("event:/Environment1");
	m_environment.SetVolume(0.45f);
	m_vulcan = m_audioComp->PlayEvent("event:/Vulcan");
	m_vulcan.Stop(false);

	m_fpsCam = new FPSCamera(this);
	m_fpsCam->SetActive();

	m_meshComp = new MeshComponent(this);
	m_gunModel = new GameObject();
	m_gunModel->SetScale(1.7f);
	m_meshComp = new MeshComponent(m_gunModel);
	m_meshComp->SetMesh(D3D.GetMesh("Assets/Models/Player_Gun.gpmesh"));
	m_meshComp->SetMaterial(material::MetalBlack);

	m_boxComp = new BoxComponent(this);
	AABB box(Vector3(13.0f, 15.0f, 12.0f), Vector3(-13.0f, -17.0f, -12.0f));
	m_boxComp->SetObjectBox(box);
}

void FPSObject::UpdateGameObject(float deltaTime)
{
	auto m = m_moveComp->GetMoveAmount();
	m_lastFootStep += m.LengthSq();
	m_lastShot += deltaTime;
	m_invincibleTime -= deltaTime;
	m_hitMarkerDisplay -= deltaTime;
	m_stepCharge += deltaTime;
	m_stepCharge = std::clamp(m_stepCharge, 0.0f, m_oneStepSec * 2.0f);

	FixAllBoxes();

	//足音
	auto exVel = m_moveComp->GetExceptionVel();
	if(m_lastFootStep >= 140.0f &&
		exVel.LengthSq() < 500.0f &&
		m_charaState == OnGround)
	{
		PlayFootStep();
	}

	//射撃
	if (m_shoot)
	{
		if (m_lastShot >= m_shootingRate)
		{
			Shoot();
			m_lastShot = 0;
		}

		if (!m_vulcan.IsValid())
		{
			m_vulcan = m_audioComp->PlayEvent("event:/Vulcan");
		}
	}
	else
	{
		m_vulcan.Stop();
	}

	//モデル追従
	const Vector3 modelOffset(Vector3(4.0f, -6.0f, 6.0f));
	Vector3 modelPos = GetPosition();
	modelPos += GetForward() * modelOffset.z;
	modelPos += GetRight() * modelOffset.x;
	modelPos.y += modelOffset.y;
	m_gunModel->SetPosition(modelPos);
	//ヨー回転→ピッチ回転
	auto q = Quaternion::Concatenate(
		GetRotation(), 
		Quaternion(GetRight(), m_fpsCam->GetCurPitch())
	);
	m_gunModel->SetRotation(q);
}

void FPSObject::ObjectInput(const InputState& state)
{
	Vector3 moveDir = Vector3::Zero;
	m_moveComp->SetMaxSpeed(150.0f);
	//移動系入力
	if (state.Keyboad.GetKeyState(VK_W) == Held)
	{
		moveDir += GetForward();
	}
	if (state.Keyboad.GetKeyState(VK_S) == Held)
	{
		moveDir -= GetForward();
	}
	if (state.Keyboad.GetKeyState(VK_A) == Held)
	{
		moveDir -= GetRight();
	}
	if (state.Keyboad.GetKeyState(VK_D) == Held)
	{
		moveDir += GetRight();
	}

	moveDir.Normalize();

	if (state.Keyboad.GetKeyState(VK_LSHIFT) == Pressed &&
		m_stepCharge >= m_oneStepSec)
	{
		Vector3 add = moveDir * 30000.0f;;
		if (moveDir.IsZero())
			add = GetForward() * 30000.0f;

		m_moveComp->AddStrikeAccel(add);
		m_stepCharge -= m_oneStepSec;
		m_audioComp->PlayEvent("event:/Dash");
		m_fpsCam->Wiggle(0.0f, 0.97f, 150.0f);
	}

	//ジャンプ入力
	if (state.Keyboad.GetKeyState(VK_SPACE) == Held &&
		m_charaState == OnGround)
	{
		if (m_jumpCharge == 0.0f) {
			m_charge = m_audioComp->PlayEvent("event:/Charge1");
		}
		m_jumpCharge += 1000.0f;
		m_jumpCharge = std::clamp(m_jumpCharge, 20000.0f, 150000.0f);
		m_fpsCam->Wiggle(0.0f, 0.5f, 15.0f);
	}
	else if (state.Keyboad.GetKeyState(VK_SPACE) == Released &&
		m_charaState == OnGround)
	{
		auto add = Vector3::Up * m_jumpCharge;
		m_moveComp->AddStrikeAccel(add);
		m_jumpCharge = 0.0f;
		m_charge.Stop();
		m_audioComp->PlayEvent("event:/Jump");
		m_fpsCam->Wiggle(0.0f, 0.95f, 150.0f);
	}
	else
	{
		m_jumpCharge = 0.0f;
		m_charge.Stop();
	}

	//武装系入力
	if (state.Keyboad.GetKeyState(VK_LBUTTON) == Pressed &&
		m_remainAmmo > 0)
	{
		m_shoot = true;
	}
	else if (state.Keyboad.GetKeyState(VK_LBUTTON) == Released ||
		m_remainAmmo <= 0)
	{
		m_shoot = false;
	}

	if (GAME.GetState() != Game::GamePlay)
	{
		DEBUG_LOG("FPSObject:detectPause\n");
	}

	//移動速度設定
	auto accel = moveDir * m_accelAmount;
	if (m_jumpCharge > 0.0f) accel *= 0.5f;
	m_moveComp->SetAccel(accel);
	
	//視点移動速度設定
	Vector2 mouseVec = state.mouse.GetRelativeVec();
	const int maxMouseSpeed = 500;
	const float maxAngularSpeed = PI * 8;
	float angularSpeed = 0.0f;
	if (mouseVec.x != 0)
	{
		//-1 ~ 1にスケーリング
		angularSpeed = mouseVec.x / maxMouseSpeed;
		//最高速度を乗算
		angularSpeed *= maxAngularSpeed;
	}
	m_moveComp->SetAngularSpeed(angularSpeed);

	const float maxPitchSpeed = PI * 8;
	float pitchSpeed = 0.0f;
	if (mouseVec.y != 0)
	{
		pitchSpeed = mouseVec.y / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	m_fpsCam->SetPitchSpeed(pitchSpeed);
}

void FPSObject::Shoot()
{
	if (m_remainAmmo <= 0) return;

	//着弾点計算
	Vector3 start, dir;
	CollisionInfo info;
	D3D.GetScreenDirection(start, dir);
	Vector3 targetPos = start + dir * 500.0f;
	if (GAME.GetPhysWorld()->SegmentCast(LineSegment(start, targetPos), info, m_boxComp)) {
		targetPos = info.collidePoint;
	}

	//発射位置設定
	Vector3 bulletStart = m_gunModel->GetPosition() + 
		Vector3::Rotate(Vector3(0.0f, -0.1f, 20.0f), m_gunModel->GetRotation()); //オフセット

	//拡散設定
	std::uniform_real_distribution<> sDist(-m_maxSpread, m_maxSpread);
	auto bulletForce = Vector3::Normalized(targetPos - bulletStart) * 4500.0f;
	bulletForce += Vector3(float(sDist(m_rnd)), float(sDist(m_rnd)), 0.0f);

	Bullet* bullet = new Bullet(this, 0.045f); //45g
	bullet->SetPosition(bulletStart);
	bullet->SetRotation(m_gunModel->GetRotation());
	bullet->AddStrikeForce(bulletForce);
	m_remainAmmo--;

	m_fpsCam->Wiggle(0.0f, 0.8f, 11.0f);
}

void FPSObject::HitEvent()
{
	m_audioComp->PlayEvent("event:/Hit");
	m_hitMarkerDisplay = MARKER_DISPLAY;
}

void FPSObject::Dead()
{
	SetState(Paused);
	m_environment.Stop();
	m_shoot = false;
	m_vulcan.Stop(true);
	m_dying.Stop();
	m_charge.Stop();
	new GameOver();
}

void FPSObject::SetPloxAlert(GameObject* obj)
{
	auto enemy = dynamic_cast<EnemyObject*>(obj);
	if (enemy)
	{
		Vector3 toEnemy = obj->GetPosition() - this->GetPosition();

		if (toEnemy.LengthSq() <= m_alertDist * m_alertDist)
		{
			m_closeEnemies.emplace_back(enemy);

			if (!m_plox.IsValid()) 
			{
				m_plox = m_audioComp->PlayEvent("event:/Alert_Plox");
			}
		}
	}
}

bool FPSObject::ContactEnemy(GameObject* obj)
{
	auto enemy = dynamic_cast<EnemyObject*>(obj);
	if (enemy)
	{
		//ダメージ処理
		if (m_invincibleTime <= 0.0f && m_hp > 0.0f)
		{
			m_hp -= enemy->g_giveDamage;
			m_invincibleTime = 1.0f;
			m_audioComp->PlayEvent("event:/Damage");
			m_fpsCam->Wiggle(0.0f, 0.90f, 8000.0f);

			m_damagedEnemy = enemy;

			if (m_hp <= 30.0f && !m_dying.IsValid())
			{
				m_dying = m_audioComp->PlayEvent("event:/Alert_Dying");
			}
			if (m_hp <= 0.0f) Dead();
		}
		return true;
	}

	return false;
}

void FPSObject::FixAllBoxes()
{
	if (m_boxComp == nullptr)
	{
		DEBUG_LOG("m_boxComp is null\n");
		return;
	}

	//自身のboxComponentの状態を更新しておく
	ComputeWorldTransform();

	const AABB& thisBox = m_boxComp->GetWorldBox();
	Vector3 pos = GetPosition();

	LineSegment downLs = LineSegment(pos, pos - Vector3::Up*21.0f);
	float outT = 0.0f;
	Vector3 outNorm;
	m_charaState = InAir;
	m_moveComp->SetUseGravity(true);

	//距離が近い敵リストを初期化
	m_closeEnemies.clear();

	auto& boxComps = GAME.GetPhysWorld()->GetBoxComps();
	for (auto boxComp : boxComps)
	{
		if (boxComp == m_boxComp) continue;

		//接地判定
		const AABB& otherBox = boxComp->GetWorldBox();
		if (Intersect(downLs, otherBox, outT, outNorm)) 
		{
			m_charaState = OnGround;
			m_moveComp->SetUseGravity(false);
		}
		//位置修正処理
		if (Intersect(otherBox, thisBox))
		{
			//敵と接触した場合は位置修正しない
			if (ContactEnemy(boxComp->GetOwner())) {
				continue;
			}

			//各軸方向で最小の差が接触したボックス間の距離ということになる(最大、最小位置同士の接触はありえない)
			float dx1 = otherBox.m_max.x - thisBox.m_min.x;
			float dx2 = otherBox.m_min.x - thisBox.m_max.x;
			float dy1 = otherBox.m_max.y - thisBox.m_min.y;
			float dy2 = otherBox.m_min.y - thisBox.m_max.y;
			float dz1 = otherBox.m_max.z - thisBox.m_min.z;
			float dz2 = otherBox.m_min.z - thisBox.m_max.z;

			float dx = (std::abs(dx1) < std::abs(dx2)) ? dx1 : dx2;
			float dz = (std::abs(dz1) < std::abs(dz2)) ? dz1 : dz2;
			float dy = (std::abs(dy1) < std::abs(dy2)) ? dy1 : dy2;

			if (std::abs(dx) <= std::abs(dy) && std::abs(dx) <= std::abs(dz))
			{
				pos.x += dx;
			}
			else if (std::abs(dy) <= std::abs(dx) && std::abs(dy) <= std::abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			SetPosition(pos);
			m_boxComp->OnUpdateWorldTransform();
		}
		else
		{
			//接近警報設定
			SetPloxAlert(boxComp->GetOwner());
		}
	}
}

void FPSObject::PlayFootStep()
{
	m_footStep.SetPaused(false);
	m_footStep.Restart();
	m_lastFootStep = 0.0f;
	m_fpsCam->Wiggle(0.0001f, 0.85f, Vector2(5.0f, 75.0f));
}
