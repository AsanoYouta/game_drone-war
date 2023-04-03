#pragma once
#include "framework.h"
#include "Source/Math/mymath.h"
#include "Source/DirectX/DirectX.h"
#include "PhysWorld.h"

class GameObject
{
public:
	//状態管理
	enum State
	{
		Active,
		Paused,
		Dead
	};
	enum Type
	{
		Dynamic,
		Static
	};

	GameObject(size_t maxInstanceNum = 5, Type type = Dynamic);
	virtual ~GameObject();

	//Get/Set関数
	Type GetType() const { return m_type; }
	State GetState() const { return m_state; }
	const Vector3& GetPosition() const { return m_position; }
	float GetScale() const { return m_scale; }
	const Quaternion& GetRotation() const { return m_rotation; }
	Vector3 GetUp() const { return Vector3::Rotate(Vector3::Up, m_rotation); }
	Vector3 GetForward() const { return Vector3::Rotate(Vector3::Forward, m_rotation); }
	Vector3 GetRight() const { return Vector3::Rotate(Vector3::Right, m_rotation); }
	const XMMATRIX& GetWorldTransform() const { return m_worldTransform; }
	const std::vector<class ObjectInstance*> GetInstances() const { return m_instances; }
	ID3D11Buffer* GetInstanceBuffer() const { return m_instanceBuffer.Get(); }
	void SetState(State st) { m_state = st; }
	void SetType(Type type) { m_type = type; }
	void SetPosition(const Vector3& pos) { m_position = pos; m_recomputeWorldTransform = true; }
	void SetScale(float scale) { m_scale = scale; m_recomputeWorldTransform = true; }
	void SetRotation(const Quaternion& q) { m_rotation = q; m_recomputeWorldTransform = true; }
	void SetRecomputeWT(bool recompute) { m_recomputeWorldTransform = recompute; }
	void SetOneSync(bool sync) { m_oneSync = sync; }

	//更新
	void Update(float deltaTime);
	//各コンポーネント更新
	void UpdateComponents(float deltaTime);
	//オブジェクト固有の更新処理
	virtual void UpdateGameObject(float deltaTime);
	virtual void OnCollision(const CollisionInfo& colInfo);
	//インスタンスの更新
	void UpdateInstances(float deltaTime);

	//入力処理
	void ProcessInput(const struct InputState& state);
	virtual void ObjectInput(const struct InputState& state);

	//コンポーネント/インスタンス管理
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
	void AddInstance(class ObjectInstance* instance);
	void RemoveInstance(class ObjectInstance* instance);

	//ワールド行列計算
	void ComputeWorldTransform();

	void RotateToNewForward(const Vector3& forward);

protected:
	//インスタンシングのためのバッファ作成/更新
	bool CreateInstanceBuffer();
	void UpdateInstaceBuffer();

	size_t m_maxInstance;
	ComPtr<ID3D11Buffer> m_instanceBuffer;
	std::vector<class ObjectInstance*> m_instances;

private:
	State m_state;
	Type m_type;
	bool m_oneSync; //インスタンスを必ず一つ同期するか
	Vector3 m_position;
	Quaternion m_rotation;
	float m_scale;
	XMMATRIX m_worldTransform;
	bool m_recomputeWorldTransform = true;

	std::vector<class Component*> m_components;
};

