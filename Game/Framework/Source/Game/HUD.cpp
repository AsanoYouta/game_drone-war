#include "HUD.h"
#include "Game.h"
#include "MoveComponent2.h"
#include "FPSCamera.h"
#include "GameManager.h"
#include "EnemyObject.h"
#include "Slider.h"
#include "FPSObject.h"
#include "Source/Math/mymath.h"
#include <sstream>
#include <iomanip>

HUD::HUD()
	:UIScreen()
{
	m_playerObj = dynamic_cast<FPSObject*>(GAME.GetPlayer());

	m_crosshair.reset(new Bitmap(L"Assets/Textures/Crosshair.png", Vector2(0.5f, 0.5f)));

	m_damageAlert.reset(new Bitmap(L"Assets/Textures/DamageAlert.png", Vector2(0.5f, 0.5f), 1.0f));

	m_hitMarker.reset(new Bitmap(L"Assets/Textures/Hitmarker.png", Vector2(0.5f, 0.5f), 1.0f));
	m_hitMarker->SetRotation(45.0f);

	//接近警報の同時表示上限５
	for (UINT i = 0; i < 5; i++)
	{
		m_ploxAlerts.emplace_back(
			new Bitmap(L"Assets/Textures/ProximityAlert.png", Vector2(0.5f, 0.5f), 0.6f)
		);
	}

	//ピッチメーター生成
	float dist = 2.5f; //ピッチメーター間の間隔
	for (UINT i = 0; i < 10 - 1; i++)
	{
		float yPos = ((float(i) / 10.0f) * dist) - 0.5f;
		float size = 0.58f;
		if (i == 4) size = 0.65f;
		m_pitchMeters.emplace_back(
			new Bitmap(L"Assets/Textures/PitchMeter.png", Vector2(0.5f, yPos), size),
			yPos
		);
	}

	m_step1.reset(new Slider(Vector2(0.39f, 0.85f), 0.1f, 0.014f));
	m_step2.reset(new Slider(Vector2(0.51f, 0.85f), 0.1f, 0.014f));

	m_arm.reset(new Text(L"GUN", Vector2(0.1f, 0.85f), D2D.g_hudFormat));
	m_armVal.reset(new Text(L"0", Vector2(0.15f, 0.85f), D2D.g_hudFormat));

	m_dmg.reset(new Text(L"DMG", Vector2(0.1f, 0.9f), D2D.g_hudFormat));
	m_dmgVal.reset(new Text(L"0%", Vector2(0.15f, 0.9f), D2D.g_hudFormat));

	m_speed.reset(new Text(L"SPEED", Vector2(0.5f - 0.27f, 0.5f), D2D.g_hudFormat2));
	m_speedVal.reset(new Text(L"000", Vector2(0.5f - 0.2f, 0.5f), D2D.g_hudFormat2));
	m_speedVal->SetDrawRect(true);

	m_alt.reset(new Text(L"ALT", Vector2(0.51f + 0.24f, 0.5f), D2D.g_hudFormat2));
	m_altVal.reset(new Text(L"00000", Vector2(0.5f + 0.2f, 0.5f), D2D.g_hudFormat2));
	m_altVal->SetDrawRect(true);

	m_jump.reset(new Text(L"JUMP", Vector2(0.5f - 0.01f, 0.7f), D2D.g_hudFormat2));
	m_jumpVal.reset(new Text(L"000", Vector2(0.5f, 0.75f), D2D.g_hudFormat2));
	m_jumpVal->SetDrawRect(true);

	m_time.reset(new Text(L"TIME", Vector2(0.095f, 0.11f), D2D.g_hudFormat));
	m_timeVal.reset(new Text(L"00:00:00", Vector2(0.16f, 0.11f), D2D.g_hudFormat));

	m_wave.reset(new Text(L"WAVE", Vector2(0.095f, 0.155f), D2D.g_hudFormat));
	m_waveVal.reset(new Text(L"0/0", Vector2(0.16f, 0.155f), D2D.g_hudFormat));

	m_guide_move.reset(new Text(L"WASD：移動", Vector2(0.90f, 0.775f), D2D.g_hudFormat2));
	m_guide_shoot.reset(new Text(L"マウス左ボタン：射撃", Vector2(0.91f, 0.825f), D2D.g_hudFormat2));
	m_guide_dash.reset(new Text(L"LSHIFT：ブースト", Vector2(0.895f, 0.875f), D2D.g_hudFormat2));
	m_guide_jump.reset(new Text(L"Space：ジャンプ", Vector2(0.892f, 0.925f), D2D.g_hudFormat2));

	m_rect1 = D2D.CreateRect(Vector2(0.5f, 0.5f), 0.25f, 0.7f);
	m_rect2 = D2D.CreateRect(Vector2(0.5f, 0.5f), 0.7f, 0.6f);
}

void HUD::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);
}

void HUD::Draw()
{
	m_playerObj = dynamic_cast<FPSObject*>(GAME.GetPlayer());
	if (!m_playerObj ||
		GAME.GetState() == Game::State::Paused) return;

	//クロスヘア表示
	m_crosshair->Draw();

	//ヒットマーカー表示
	float dispTime = m_playerObj->GetHitMarkerDisplay();
	if (dispTime > 0)
	{
		float scale = dispTime / MARKER_DISPLAY;
		m_hitMarker->SetScale(scale);
		m_hitMarker->Draw();
	}

	//ピッチメーター表示
	float pitch = m_playerObj->GetFPSCam()->GetCurPitch();
	float maxPitch = m_playerObj->GetFPSCam()->GetMaxPitch();
	for (auto m : m_pitchMeters)
	{
		float newY = m.second - (pitch / maxPitch);
		m.first->SetNaturalRect(Vector2(0.5f, newY));
		m.first->Draw();
	}

	//ステップゲージ表示
	float stepCharge = m_playerObj->GetStepCharge();
	const float oneStep = m_playerObj->GetOneStepSec();
	float step1Val = stepCharge;
	step1Val = std::clamp(step1Val, 0.0f, oneStep);
	float step2Val = stepCharge - oneStep;
	if (step2Val < 0) step2Val = 0.0f;
	m_step1->Draw(step1Val / oneStep);
	m_step2->Draw(step2Val / oneStep);

	//接近警報表示
	Vector3 forward = m_playerObj->GetFPSCam()->GetForward();
	forward.Normalize();
	auto closes = m_playerObj->GetCloseEnemies();
	for (UINT i = 0; i < m_ploxAlerts.size(); i++)
	{
		if (!closes.empty() && i < closes.size()) 
		{
			auto enemy = closes[i];
			if (!enemy) continue;

			Vector3 toEnemy = enemy->GetPosition() - m_playerObj->GetPosition();
			toEnemy.Normalize();

			float dot = Vector3::Dot(forward, toEnemy);
			//DEBUG_LOG("dot %f\n", dot);
			auto cross = Vector3::Cross(forward, toEnemy);
			float degree = math::ToDegree(std::acosf(dot));
			if (cross.y < 0) degree = 360.0f - degree;
			//if (cross.x < 0) degree = 180.0f - degree;
			//DEBUG_LOG("cross %f, %f, %f\n", cross.x, cross.y, cross.z);

			m_ploxAlerts[i]->SetRotation(degree);
			m_ploxAlerts[i]->Draw();
		}
	}

	//ダメージ方向表示
	auto dEnemy = m_playerObj->GetDamagedEnemy();
	float invTime = m_playerObj->GetInvincibleTime();
	if (dEnemy != nullptr && invTime > 0)
	{
		Vector3 toEnemy = dEnemy->GetPosition() - m_playerObj->GetPosition();
		toEnemy.Normalize();
		float dot = Vector3::Dot(forward, toEnemy);
		auto cross = Vector3::Cross(forward, toEnemy);
		float degree = math::ToDegree(std::acosf(dot));
		if (cross.y < 0) degree *= -1.0f;

		m_damageAlert->SetRotation(degree);
		m_damageAlert->Draw();
	}

	//残弾表示
	std::wstring wstr;
	wstr = std::to_wstring(m_playerObj->GetRemainBullets());
	m_armVal->SetText(wstr);
	m_armVal->Draw();
	m_arm->Draw();

	//ライフ表示
	auto damage = int(100.0f - m_playerObj->GetHp());
	wstr = std::to_wstring(damage);
	m_dmgVal->SetText(wstr + L"%");
	if (damage >= 70) m_dmgVal->SetBrush(D2D.g_redBrush);
	m_dmgVal->Draw();
	m_dmg->Draw();

	//スピード表示
	auto speed = m_playerObj->GetMoveComp()->GetMoveAmount().Length();
	wstr = std::to_wstring(int(speed*60.0f));
	m_speedVal->SetText(wstr);
	m_speed->Draw();
	m_speedVal->Draw();

	//高度表示
	wstr = std::to_wstring(m_playerObj->GetPosition().y);
	for (int i = 0; i < wstr.size() - 1; i++) wstr.pop_back();
	m_altVal->SetText(wstr);
	m_alt->Draw();
	m_altVal->Draw();

	//ジャンプゲージ表示
	int jumpVal = int((m_playerObj->GetJumpCharge() / 150000.0f) * 100.0f);
	wstr = std::to_wstring(jumpVal);
	if (jumpVal > 0)
	{
		if (jumpVal >= 100) m_jumpVal->SetBrush(D2D.g_onButtonBrush);
		else m_jumpVal->SetBrush(D2D.GetDefBrush());

		m_jumpVal->SetText(wstr);
		m_jump->Draw();
		m_jumpVal->Draw();
	}

	//タイム/ウェーブ数表示
	auto gameManager = m_playerObj->GetGameManager();
	if (gameManager)
	{
		auto time = gameManager->GetGameTime();
		if (time <= 0.0f) m_timeVal->SetText(L"00:00:00");
		else
		{
			wstr = math::ConvertToTimeRotation(time);
			m_timeVal->SetText(wstr);
		}
		if (gameManager->GetInGame() == false) m_timeVal->SetBrush(D2D.g_onButtonBrush);
		else m_timeVal->SetBrush(D2D.GetDefBrush());

		auto enemyManager = gameManager->GetEnemyManager();
		auto cWave = enemyManager->GetCurrentWave();
		auto maxWave = enemyManager->GetWaveNum();
		wstr = std::to_wstring(cWave) + L" / " + std::to_wstring(maxWave);
		m_waveVal->SetText(wstr);
	}
	m_time->Draw();
	m_timeVal->Draw();
	m_wave->Draw();
	m_waveVal->Draw();

	//装飾系
	//D2D.GetRT()->DrawRectangle(m_rect1, D2D.GetDefBrush().Get());
	D2D.GetRT()->DrawRectangle(m_rect2, D2D.GetDefBrush().Get());

	//操作ガイド
	m_guide_move->Draw();
	m_guide_shoot->Draw();
	m_guide_dash->Draw();
	m_guide_jump->Draw();
}