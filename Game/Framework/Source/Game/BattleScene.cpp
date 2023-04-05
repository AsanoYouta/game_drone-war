#include "BattleScene.h"
#include "Scene.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "GameManager.h"
#include "HUD.h"
#include "PauseMenu.h"
#include "FPSObject.h"
#include "EnemyManager.h"
#include "EnemyObject.h"
#include "SwarmEnemy.h"
#include "PlaneObject.h"
#include "Floor.h"
#include "Building.h"
#include "SquareBuilding.h"
#include "OblongBuilding.h"
#include "Wall.h"
#include "Game.h"
#include "SkySphere.h"

BattleScene::BattleScene(const std::string& name)
	:Scene(name)
{

}

void BattleScene::Load()
{
	//����������
	GAME.SetState(Game::State::GamePlay);
	GAME.GetInputSystem()->SetRelativeMode(true);
	m_skyColor = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	//-----------------------------
	//�ÓI�I�u�W�F�N�g����
	//-----------------------------
	GameObject* obj = new GameObject();

	//��
	const float blockSize = 300.0f;
	int widthNum = 6;
	float width = widthNum * blockSize;
	float start = blockSize * 0.5f;
	for (int i = 0; i < widthNum; i++)
	{
		for (int j = 0; j < widthNum; j++)
		{
			obj = new Floor(2);
			obj->SetPosition(Vector3(start + j * blockSize, 10.0f, start + i * blockSize));
			obj->SetScale(3.0f);
		}
	}
	float scale = 1000.0f;
	obj = new Floor(3);
	obj->SetPosition(Vector3(width+scale, -scale - 30.0f, width+scale));
	obj->SetScale(scale);
	//��Q��
	for (int i = 0; i < widthNum; i++)
	{
		for (int j = 0; j < widthNum; j++)
		{
			float x = start + j * blockSize;
			float z = start + i * blockSize;
			float maxH = float(widthNum)*0.5f;
			float l = float(Building::typeNum) - ((float(i) - maxH) * (float(i) - maxH))*0.7f;
			l = std::roundf(l);
			//DEBUG_LOG("l %f\n", l);
			PlaceRandBuilding(blockSize, Vector3(x, 10.0f, z), UINT(l), 1);
		}
	}

	//�w�i����
	auto skybox = new SkySphere(3);
	skybox->SetPosition(Vector3(width, 0, width));

	//�X�e�[�W�̋��E�ǐ���
	//���[
	auto max = Vector3(0.0f, 5000.0f, width);
	auto min = Vector3(-100.0f, -5000.0f, 0.0f);
	new Wall(max, min);
	//�E�[
	max = Vector3(width + 100.0f, 5000.0f, width);
	min = Vector3(width, -5000.0f, 0.0f);
	new Wall(max, min);
	//���[
	max = Vector3(width, 5000.0f, 0.0f);
	min = Vector3(0.0f, -5000.0f, -100.0f);
	new Wall(max, min);
	//��[
	max = Vector3(width, 5000.0f, width + 100.0f);
	min = Vector3(0.0f, -5000.0f, width);
	new Wall(max, min);

	//���C�e�B���O�f�[�^�Z�b�g
	D3D.SetAmbientLight(Vector3(0.3f, 0.3f, 0.3f));
	DirectionalLight dir;
	dir.dir = XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f);
	dir.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	dir.specColor = XMFLOAT4(0.7f, 0.7f, 0.7f, 0.0f);
	D3D.SetDirectionalLight(dir);

	//-----------------------------
	//���I�I�u�W�F�N�g����
	//-----------------------------
	
	//�v���C���[����
	auto hud = new HUD();
	auto player = new FPSObject(); //�R���X�^���N�^�ŃJ������SetActive�����
	GAME.SetPlayer(player);
	player->SetPosition(Vector3(width*0.5f, 600.0f, width*0.5f));

	//�Q�[���V�X�e������
	auto gameManager = new GameManager();

	//�G����
	float height = 1200.0f;
	auto enemyManager = new EnemyManager(gameManager,4);
	enemyManager->SetEnemyIncreseNum(15);
	enemyManager->SetIncreseSpeed(25.0f);
	enemyManager->AddSpawnPos(Vector3(width * 0.5f, height, width - 200.0f));
	enemyManager->AddSpawnPos(Vector3(200.0f, height, 200.0f));
	enemyManager->AddSpawnPos(Vector3(width - 200.0f, height, 200.0f));
	enemyManager->AddSpawnPos(Vector3(200.0f, height, width - 200.0f));
	enemyManager->AddSpawnPos(Vector3(width - 200.0f, height, width - 200.0f));

	gameManager->StartGame();
}

void BattleScene::Unload()
{
	GAME.SetPlayer(nullptr);
}

void BattleScene::ProcessInput(const InputState& state)
{
	if (state.Keyboad.GetKeyState(VK_ESCAPE) == Pressed &&
		GAME.GetState() == Game::State::GamePlay &&
		GAME.GetPlayer()->GetState() == GameObject::State::Active) 
	{
		new PauseMenu(); //Game::State = Paused
	}

	Scene::ProcessInput(state);
}
