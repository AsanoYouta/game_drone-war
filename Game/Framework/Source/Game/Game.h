#pragma once

#include "framework.h"
#include "Source/DirectX/DirectX.h"
#include "GameObject.h"
#include "SoundEvent.h"

class Game
{
public:
	enum State
	{
		GamePlay,
		Paused,
		Quit
	};

	bool Initialize(HWND hWnd, int width, int height);
	void RunLoop();
	MSG Shutdown();

	//Get/Set
	void SetState(State state) { m_state = state; }
	void SetActiveCam(class CameraComponent* cam) { m_activeCam = cam; }
	State GetState() const { return m_state; }
	class PhysWorld* GetPhysWorld() const { return m_physWorld; }
	class AudioSystem* GetAudioSystem() const { return m_audioSystem; }
	class InputSystem* GetInputSystem() const { return m_inputSystem; }
	class CameraComponent* GetActiveCam() const { return m_activeCam; }
	class Scene* GetActiveScene() const { return m_activeScene; }

	//�I�u�W�F�N�g�ǉ�/�r��
	void AddGameObject(class GameObject* gameObj);
	void RemoveGameObject(class GameObject* gameObj);

	//�V�[���ǂݍ���
	bool LoadScene(const std::string& name);
	bool ReloadScene();

	//UI�Ǘ�
	const std::vector<class UIScreen*>& GetUIStack() { return m_UIStack; };
	void PushUI(class UIScreen* screen);

	//�Q�[���R�[�h
	void SetPlayer(class GameObject* player) { m_player = player; }
	class GameObject* GetPlayer() { return m_player; }

private:
	State m_state;
	MSG m_msg;
	DWORD m_ticksCount = 0;
	bool m_updatingObjects = false;						//�I�u�W�F�N�g�X�V��(m_gameObjects�Q�ƒ�)�ɒǉ�����̂������t���O
	std::vector<class GameObject*> m_gameObjects;
	std::vector<class GameObject*> m_pendingObjects;	//�ǉ��ҋ@���I�u�W�F�N�g
	class InputSystem* m_inputSystem;
	class PhysWorld* m_physWorld;
	class AudioSystem* m_audioSystem;
	class CameraComponent* m_activeCam;
	//�A�N�e�B�u�ȃV�[��
	class Scene* m_activeScene;
	//UI�X�^�b�N�z��
	std::vector<class UIScreen*> m_UIStack;

	//�Q�[���R�[�h
	GameObject* m_player;

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void UnloadScene();

//----------------------------------------
//�V���O���g��
	static inline Game* instance;
	Game();
public:
	static void CreateInstance()
	{
		DeleteInstance();
		instance = new Game();
	}
	static void DeleteInstance()
	{
		if (instance != nullptr) {
			delete instance;
			instance = nullptr;
		}
	}
	static Game& GetInstance()
	{
		return *instance;
	}
};

#define GAME Game::GetInstance()

