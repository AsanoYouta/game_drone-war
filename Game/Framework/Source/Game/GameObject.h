#pragma once
#include "framework.h"
#include "Source/Math/mymath.h"
#include "Source/DirectX/DirectX.h"
#include "PhysWorld.h"

class GameObject
{
public:
	//��ԊǗ�
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

	//Get/Set�֐�
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

	//�X�V
	void Update(float deltaTime);
	//�e�R���|�[�l���g�X�V
	void UpdateComponents(float deltaTime);
	//�I�u�W�F�N�g�ŗL�̍X�V����
	virtual void UpdateGameObject(float deltaTime);
	virtual void OnCollision(const CollisionInfo& colInfo);
	//�C���X�^���X�̍X�V
	void UpdateInstances(float deltaTime);

	//���͏���
	void ProcessInput(const struct InputState& state);
	virtual void ObjectInput(const struct InputState& state);

	//�R���|�[�l���g/�C���X�^���X�Ǘ�
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
	void AddInstance(class ObjectInstance* instance);
	void RemoveInstance(class ObjectInstance* instance);

	//���[���h�s��v�Z
	void ComputeWorldTransform();

	void RotateToNewForward(const Vector3& forward);

protected:
	//�C���X�^���V���O�̂��߂̃o�b�t�@�쐬/�X�V
	bool CreateInstanceBuffer();
	void UpdateInstaceBuffer();

	size_t m_maxInstance;
	ComPtr<ID3D11Buffer> m_instanceBuffer;
	std::vector<class ObjectInstance*> m_instances;

private:
	State m_state;
	Type m_type;
	bool m_oneSync; //�C���X�^���X��K����������邩
	Vector3 m_position;
	Quaternion m_rotation;
	float m_scale;
	XMMATRIX m_worldTransform;
	bool m_recomputeWorldTransform = true;

	std::vector<class Component*> m_components;
};

