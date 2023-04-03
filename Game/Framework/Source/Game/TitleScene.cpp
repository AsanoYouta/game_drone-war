#include "TitleScene.h"
#include "Game.h"
#include "TitleMenu.h"
#include "CameraObject.h"
#include "SkySphere.h"
#include "Floor.h"
#include "Building.h"
#include "EnemyObject.h"
#include "SwarmEnemy.h"
#include "PhysicalMove.h"
#include "CameraComponent.h"
#include "AudioComponent.h"

TitleScene::TitleScene(std::string name)
	:Scene(name)
	,m_camObj(nullptr)
{

}

void TitleScene::Load()
{
	//初期化処理
	GAME.SetState(Game::State::GamePlay);
	GAME.GetInputSystem()->SetRelativeMode(false);
	m_skyColor = XMFLOAT4(0.1f, 0.4f, 0.6f, 1.0f);

	//-----------------------------
	//動的オブジェクト生成
	//-----------------------------
	m_camObj = new CameraObject();
	m_camObj->SetPosition(Vector3::Zero);
	m_camObj->SetActive();
	m_camObj->RotateToNewForward(Vector3::Right);

	float height = 30;
	GameObject* obj;

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			PlaceFormationEnemy(Vector3(100 + j * 500.0f, height, -4000 + i * 500.0f));
			height *= -1.2f;
		}
		height *= -1.0f;
	}
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			PlaceFormationEnemy(Vector3(100 + j * 500.0f, height, -16000 + i * 500.0f));
			height *= -1.0f;
		}
		height += 100.0f;
		height *= -1.0f;
	}
	

	//-----------------------------
	//静的オブジェクト生成
	//-----------------------------
	new TitleMenu();
	new SkySphere(2);

	//ライティングデータセット
	D3D.SetAmbientLight(Vector3(0.3f, 0.3f, 0.3f));
	DirectionalLight dir;
	dir.dir = XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f);
	dir.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	dir.specColor = XMFLOAT4(0.7f, 0.7f, 0.7f, 0.0f);
	D3D.SetDirectionalLight(dir);
	
	//bgm再生
	m_bgm = m_camObj->GetAudioComp()->PlayEvent("event:/Music_Title");
	m_bgm.SetVolume(0.6f);
}

void TitleScene::Unload()
{
	m_bgm.Stop();
}

void TitleScene::Update(float deltaTime)
{
	auto q = m_camObj->GetRotation();
	auto p = Quaternion(Vector3::Up, deltaTime*0.05f);
	auto newQ = Quaternion::Concatenate(q, p);
	m_camObj->SetRotation(newQ);

	for (auto enemy : m_enemies)
	{
		enemy->GetPhysicalMove()->AddStrikeForce(enemy->GetForward() * 70.0f);

		if (Vector3::DistanceSq(enemy->GetPosition(), m_camObj->GetPosition()) <= 100.0f * 100.0f)
		{
			m_camObj->GetCameraComp()->Wiggle(0.0, 0.94f, 170.0f);
		}
	}
}

void TitleScene::PlaceFormationEnemy(const Vector3& pos)
{
	int w = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = -w; j <= w; j++)
		{
			auto e = new EnemyObject();
			e->SetPosition(Vector3(pos.x + j * 70.0f, pos.y, pos.z - i * 100.0f));
			e->SetMaxSpeed(1000.0f);
			e->GetPhysicalMove()->AddForce(e->GetForward() * 1000.0f, 0.2f);
			//e->SetTargetObject(m_camObj);
			m_enemies.emplace_back(e);
		}
		w++;
	}
}

