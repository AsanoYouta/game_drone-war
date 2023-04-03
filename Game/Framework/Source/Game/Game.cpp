#include "framework.h"
#include "Game.h"
#include "InputSystem.h"
#include "PhysWorld.h"
#include "AudioSystem.h"
#include "Source/DirectX/DirectX.h"
#include "UIScreen.h"
#include "Scene.h"

Game::Game()
	:m_updatingObjects(false)
	,m_state(GamePlay)
	,m_activeScene(nullptr)
	,m_activeCam(nullptr)
	,m_audioSystem(nullptr)
	,m_inputSystem(nullptr)
	,m_physWorld(nullptr)
	,m_player(nullptr)
	,m_msg()
{
}

bool Game::Initialize(HWND hWnd, int width, int height)
{
	timeBeginPeriod(1); //�v�����x1ms
	m_ticksCount = timeGetTime();

	//Direct3D���Z�b�g
	Direct3D::CreateInstance();
	if (!D3D.Initialize(hWnd, width, height)) {
		DEBUG_LOG("Failed to Init D3D\n");
		return false;
	}

	//�I�[�f�B�I�V�X�e������
	m_audioSystem = new AudioSystem();
	if (!m_audioSystem->Initialize()) {
		DEBUG_LOG("Failed to Init AudioSystem\n");
		return false;
	}

	//�C���v�b�g�V�X�e������
	m_inputSystem = new InputSystem();
	if (!m_inputSystem->Initialize(hWnd))
	{
		DEBUG_LOG("Failed to initialize inputSystem\n");
		return false;
	}

	//PhysWorld(�Փ˔���Ǘ��N���X)����
	m_physWorld = new PhysWorld();

	//�V�[�����X�g������
	Scene::Initialize();
	//�V�[�����[�h
	LoadScene("Title");

	return true;
}

MSG Game::Shutdown()
{
	timeEndPeriod(1);
	if(m_audioSystem) m_audioSystem->Shutdown();
	if(m_inputSystem) m_inputSystem->Shutdown();
	UnloadScene();
	D3D.Shutdown();
	D3D.DeleteInstance();
	return m_msg;
}

void Game::RunLoop()
{
	while(m_state != Quit) 
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	if (PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (m_msg.message == WM_QUIT)
		{
			m_state = Quit;
		}
		else if (m_msg.lParam & 0x40000000) return; //���s�[�g���b�Z�[�W�͖���
		else
		{
			TranslateMessage(&m_msg);
			m_inputSystem->Update(m_msg);
			DispatchMessage(&m_msg); //msg��WndProc�ɑ��M
		}
	}
	const InputState& state = m_inputSystem->GetState();

	if (state.Keyboad.GetKeyState(VK_F1) == Pressed) {
		m_inputSystem->SetRelativeMode(true);
	}
	if (state.Keyboad.GetKeyState(VK_F2) == Pressed) {
		m_inputSystem->SetRelativeMode(false);
	}

	m_activeScene->ProcessInput(state);

	if (m_state == GamePlay)
	{
		//�e�I�u�W�F�N�g����
		m_updatingObjects = true;
		for (auto obj : m_gameObjects)
		{
			obj->ProcessInput(state);
		}
		m_updatingObjects = false;
	}
	if (!m_UIStack.empty())
	{
		//�ŏ�ʂ�UIScreen��InputState�𑗐M
		m_UIStack.back()->ProcessInput(state);
	}
}

void Game::UpdateGame()
{
	//�t���[������:16ms
	while (true)
	{
		if (timeGetTime() >= m_ticksCount + 16) break;
	}
	//�O�̃t���[���Ƃ̎��ԍ�
	float deltaTime = (timeGetTime() - m_ticksCount) / 1000.0f;
	m_ticksCount = timeGetTime();
	//�f���^�^�C������
	if (deltaTime > 0.05f) deltaTime = 0.05f;

	if (m_state == GamePlay)
	{
		//�Q�[���I�u�W�F�N�g�X�V
		m_updatingObjects = true;
		for (auto obj : m_gameObjects)
		{
			obj->Update(deltaTime);
		}
		m_updatingObjects = false;

		//�ǉ��ҋ@���I�u�W�F�N�g���N���A
		for (auto pending : m_pendingObjects)
		{
			pending->ComputeWorldTransform();
			m_gameObjects.emplace_back(pending);
		}
		m_pendingObjects.clear();

		//���񂾃I�u�W�F�N�g���ꎞ�z��ɒǉ�
		std::vector<GameObject*> deadObjects;
		for (auto obj : m_gameObjects)
		{
			if (obj->GetState() == GameObject::Dead)
				deadObjects.emplace_back(obj);
		}
		//���񂾃I�u�W�F�N�g���폜
		for (auto obj : deadObjects)
		{
			delete obj;
		}

		//�V�[���X�V
		m_activeScene->Update(deltaTime);
	}

	//�I�[�f�B�I�V�X�e���X�V
	m_audioSystem->Update(deltaTime);

	//UI�X�V
	for (auto ui : m_UIStack)
	{
		if (ui->GetState() == UIScreen::Active)
		{
			ui->Update(deltaTime);
		}
	}
	//Close��Ԃ�UI���폜
	auto it = m_UIStack.begin();
	while (it != m_UIStack.end())
	{
		if ((*it)->GetState() == UIScreen::Closing)
		{
			delete (*it);
			it = m_UIStack.erase(it);
		}
		else it++;
	}
}

void Game::GenerateOutput()
{
	D3D.Render();
}

bool Game::LoadScene(const std::string& name)
{
	if (m_activeScene)
	{
		UnloadScene();
	}
	auto scene = Scene::GetScene(name);
	if (scene) 
	{
		scene->Load();
		m_activeScene = scene;
	}
	else 
	{
		DEBUG_LOG("Failed to LoadScene : %s\n", name.c_str());
		return false;
	}
	return true;
}
bool Game::ReloadScene()
{
	if (m_activeScene)
	{
		auto scene = m_activeScene;
		UnloadScene();
		DEBUG_LOG("sceneUnloaded\n");
		scene->Load();
		m_activeScene = scene;
	}
	else 
	{
		DEBUG_LOG("Not Exists ActiveScene\n");
		return false;
	}
	return true;
}

//�S�ẴI�u�W�F�N�g��delete
void Game::UnloadScene()
{
	while (!m_gameObjects.empty())
	{
		delete m_gameObjects.back();
	}

	for (auto ui : m_UIStack)
	{
		ui->Close();
	}

	D3D.UnloadData();

	m_activeScene->Unload();
	//���ӁFstatic��map�ŊǗ����Ă��邽��delete���Ȃ�
	m_activeScene = nullptr;
}

void Game::AddGameObject(GameObject* gameObj)
{
	if (m_updatingObjects)
	{
		m_pendingObjects.emplace_back(gameObj);
	}
	else
	{
		m_gameObjects.emplace_back(gameObj);
	}
}
//�����̃I�u�W�F�N�g��delete���ꂽ�ۂɎ��s
void Game::RemoveGameObject(GameObject* gameObj)
{
	auto it = std::find(m_pendingObjects.begin(), m_pendingObjects.end(), gameObj);
	if (it != m_pendingObjects.end())
	{
		std::iter_swap(it, m_pendingObjects.end() - 1); //�Ō���Ɉړ�(Game::UnloadScene�ɂ��Ή������邽��)
		m_pendingObjects.pop_back(); //�󂢂�������������
	}
	
	it = std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObj);
	if (it != m_gameObjects.end())
	{
		std::iter_swap(it, m_gameObjects.end() - 1);
		m_gameObjects.pop_back();
	}
}

void Game::PushUI(UIScreen* screen)
{
	m_UIStack.emplace_back(screen);
}