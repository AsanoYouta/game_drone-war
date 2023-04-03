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

	//オブジェクト追加/排除
	void AddGameObject(class GameObject* gameObj);
	void RemoveGameObject(class GameObject* gameObj);

	//シーン読み込み
	bool LoadScene(const std::string& name);
	bool ReloadScene();

	//UI管理
	const std::vector<class UIScreen*>& GetUIStack() { return m_UIStack; };
	void PushUI(class UIScreen* screen);

	//ゲームコード
	void SetPlayer(class GameObject* player) { m_player = player; }
	class GameObject* GetPlayer() { return m_player; }

private:
	State m_state;
	MSG m_msg;
	DWORD m_ticksCount = 0;
	bool m_updatingObjects = false;						//オブジェクト更新中(m_gameObjects参照中)に追加するのを避けるフラグ
	std::vector<class GameObject*> m_gameObjects;
	std::vector<class GameObject*> m_pendingObjects;	//追加待機中オブジェクト
	class InputSystem* m_inputSystem;
	class PhysWorld* m_physWorld;
	class AudioSystem* m_audioSystem;
	class CameraComponent* m_activeCam;
	//アクティブなシーン
	class Scene* m_activeScene;
	//UIスタック配列
	std::vector<class UIScreen*> m_UIStack;

	//ゲームコード
	GameObject* m_player;

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void UnloadScene();

//----------------------------------------
//シングルトン
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

