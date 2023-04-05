#include "framework.h"
#include "Game.h"
#include "Component.h"
#include "GameObject.h"
#include "ObjectInstance.h"

GameObject::GameObject(size_t maxInstanceNum, Type type)
	: m_state(Active)
	, m_type(type)
	, m_oneSync(true)
	, m_position(Vector3::Zero)
	, m_rotation(Quaternion::Identity)
	, m_scale(1.0f)
	, m_recomputeWorldTransform(true)
	, m_worldTransform(XMMatrixIdentity())
	, m_maxInstance(maxInstanceNum)
{
	GAME.AddGameObject(this);
	CreateInstanceBuffer();
	//オリジナルインスタンス作成
	m_instances.emplace_back(new ObjectInstance(this));
}

GameObject::~GameObject()
{
	GAME.RemoveGameObject(this);
	while (!m_components.empty())
	{
		delete m_components.back();
		//デストラクタ処理でm_componentsからback()のコンポーネントが排除される
	}
}

bool GameObject::CreateInstanceBuffer()
{
	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth = sizeof(InstanceData) * UINT(m_maxInstance);
	ibDesc.Usage = D3D11_USAGE_DYNAMIC;
	ibDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = sizeof(InstanceData);

	if (FAILED(D3D.GetDevice()->CreateBuffer(&ibDesc, nullptr, &m_instanceBuffer))) {
		DEBUG_LOG("Failed to CreateInstanceBuffer\n");
		return false;
	}

	return true;
}

void GameObject::ProcessInput(const struct InputState& state)
{
	if (m_state == Active)
	{
		for (auto comp : m_components)
		{
			comp->ProcessInput(state);
		}

		ObjectInput(state);
	}
}

void GameObject::ObjectInput(const struct InputState& state)
{
	//継承先で設定
}

void GameObject::Update(float deltaTime)
{
	if (m_state == Active)
	{
		ComputeWorldTransform();	 
		UpdateComponents(deltaTime); //MoveComponentなどによって位置や回転が変化する
		UpdateGameObject(deltaTime);
		UpdateInstances(deltaTime);
		ComputeWorldTransform();     //他オブジェクトから与えられた変化に対応
	}
}

void GameObject::UpdateComponents(float deltaTime)
{
	for (auto comp : m_components)
	{
		comp->Update(deltaTime);
	}
}

void GameObject::UpdateInstances(float deltaTime)
{
	std::vector<ObjectInstance*> deadInstances;
	for (auto ins : m_instances)
	{
		ins->Update(deltaTime);
		if (!ins->GetIsLive())
		{
			deadInstances.emplace_back(ins);
		}
	}
	for (auto ins : deadInstances)
	{
		delete ins;
	}
}

void GameObject::UpdateGameObject(float deltaTime)
{
	//継承先で設定
}

void GameObject::ComputeWorldTransform()
{
	if (m_recomputeWorldTransform)
	{
		m_recomputeWorldTransform = false;
		m_worldTransform = XMMatrixScaling(m_scale, m_scale, m_scale);
		m_worldTransform *= XMMatrixRotationQuaternion(ConvertToXMVECTOR(m_rotation));
		m_worldTransform *= XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

		if (!m_instances.empty())
		{
			for (auto ins : m_instances)
			{
				ins->ComputeWorldTransform();
			}

			UpdateInstaceBuffer();
		}

		for (auto comp : m_components)
		{
			//衝突判定ボックスの状態更新に使う
			comp->OnUpdateWorldTransform();
		}

		//DEBUG_LOG("WTComputed ");
	}
}

void GameObject::UpdateInstaceBuffer()
{
	if (m_instances.size() > m_maxInstance) {
		DEBUG_LOG("Instance Count Exceeded\n");
	}

	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
	//MapでGPUアクセスを無効化
	if (FAILED(D3D.GetDeviceContext()->Map(m_instanceBuffer.Get(), 0, mapType, 0, &mappedBuffer))) {
		DEBUG_LOG("Failed to MapInstanceBuffer\n");
	}

	auto* insData = (InstanceData*)mappedBuffer.pData;
	//オリジナルインスタンスを同期
	if(m_oneSync) m_instances[0]->SyncOwner();
	for (size_t i = 0; i < m_instances.size(); i++)
	{
		//各インスタンスのパラメータ設定
		XMStoreFloat4x4(&(insData[i].World), XMMatrixTranspose(m_instances[i]->GetWorldTransform()));
		insData[i].Color = m_instances[i]->GetColor();
	}

	//UnmapでGPUアクセス有効化
	D3D.GetDeviceContext()->Unmap(m_instanceBuffer.Get(), 0);
}

void GameObject::OnCollision(const CollisionInfo& colInfo)
{
	//継承先で設定
}

void GameObject::AddComponent(Component* component)
{
	int myOrder = component->GetUpdateOrder();
	auto it = m_components.begin();
	for (; it != m_components.end(); ++it)
	{
		if (myOrder < (*it)->GetUpdateOrder())
		{
			break;
		}
	}

	//itの一つ前に挿入される
	m_components.insert(it, component);
}
//引数のコンポーネントがdeleteされた際に実行
void GameObject::RemoveComponent(Component* component)
{
	auto it = std::find(m_components.begin(), m_components.end(), component);
	if (it != m_components.end())
	{
		m_components.erase(it);
	}
}

void GameObject::AddInstance(ObjectInstance* instance)
{
	m_instances.emplace_back(instance);
}
void GameObject::RemoveInstance(ObjectInstance* instance)
{
	auto it = std::find(m_instances.begin(), m_instances.end(), instance);
	if (it != m_instances.end())
	{
		m_instances.erase(it);
	}
}

//オブジェクトをforwardに向かせる
void GameObject::RotateToNewForward(const Vector3& forward)
{
	//便宜上Vector3::Forwardを基準にする
	float dot = Vector3::Dot(Vector3::Forward, forward);
	float radian = std::acosf(dot);

	if (dot > 0.9999f) //ほぼ同じ方向に向いていた場合
	{
		SetRotation(Quaternion::Identity);
	}
	else if (dot < -0.9999f) //ほぼ真逆を向いていた場合
	{
		SetRotation(Quaternion(Vector3::Up, PI)); //反対を向かせる
	}
	else
	{
		//回転軸を外積で算出
		Vector3 axis = Vector3::Cross(Vector3::Forward, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, radian));
	}
}
