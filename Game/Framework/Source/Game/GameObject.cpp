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
	//�I���W�i���C���X�^���X�쐬
	m_instances.emplace_back(new ObjectInstance(this));
}

GameObject::~GameObject()
{
	GAME.RemoveGameObject(this);
	while (!m_components.empty())
	{
		delete m_components.back();
		//�f�X�g���N�^������m_components����back()�̃R���|�[�l���g���r�������
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
	//�p����Őݒ�
}

void GameObject::Update(float deltaTime)
{
	if (m_state == Active)
	{
		ComputeWorldTransform();	 
		UpdateComponents(deltaTime); //MoveComponent�Ȃǂɂ���Ĉʒu���]���ω�����
		UpdateGameObject(deltaTime);
		UpdateInstances(deltaTime);
		ComputeWorldTransform();     //���I�u�W�F�N�g����^����ꂽ�ω��ɑΉ�
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
	//�p����Őݒ�
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
			//�Փ˔���{�b�N�X�̏�ԍX�V�Ɏg��
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
	//Map��GPU�A�N�Z�X�𖳌���
	if (FAILED(D3D.GetDeviceContext()->Map(m_instanceBuffer.Get(), 0, mapType, 0, &mappedBuffer))) {
		DEBUG_LOG("Failed to MapInstanceBuffer\n");
	}

	auto* insData = (InstanceData*)mappedBuffer.pData;
	//�I���W�i���C���X�^���X�𓯊�
	if(m_oneSync) m_instances[0]->SyncOwner();
	for (size_t i = 0; i < m_instances.size(); i++)
	{
		//�e�C���X�^���X�̃p�����[�^�ݒ�
		XMStoreFloat4x4(&(insData[i].World), XMMatrixTranspose(m_instances[i]->GetWorldTransform()));
		insData[i].Color = m_instances[i]->GetColor();
	}

	//Unmap��GPU�A�N�Z�X�L����
	D3D.GetDeviceContext()->Unmap(m_instanceBuffer.Get(), 0);
}

void GameObject::OnCollision(const CollisionInfo& colInfo)
{
	//�p����Őݒ�
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

	//it�̈�O�ɑ}�������
	m_components.insert(it, component);
}
//�����̃R���|�[�l���g��delete���ꂽ�ۂɎ��s
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

//�I�u�W�F�N�g��forward�Ɍ�������
void GameObject::RotateToNewForward(const Vector3& forward)
{
	//�֋X��Vector3::Forward����ɂ���
	float dot = Vector3::Dot(Vector3::Forward, forward);
	float radian = std::acosf(dot);

	if (dot > 0.9999f) //�قړ��������Ɍ����Ă����ꍇ
	{
		SetRotation(Quaternion::Identity);
	}
	else if (dot < -0.9999f) //�قڐ^�t�������Ă����ꍇ
	{
		SetRotation(Quaternion(Vector3::Up, PI)); //���΂���������
	}
	else
	{
		//��]�����O�ςŎZ�o
		Vector3 axis = Vector3::Cross(Vector3::Forward, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, radian));
	}
}
