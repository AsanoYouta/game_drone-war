#include "SwarmEnemy.h"
#include "PhysicalMove.h"
#include "Source/Math/mymath.h"
#include "Game.h"
#include "PhysWorld.h"
#include "BoxComponent.h"

//static
std::vector<SwarmEnemy*> SwarmEnemy::s_swarm = {};

void SwarmEnemy::AddSwarmEnemy(SwarmEnemy* enemy)
{
	s_swarm.emplace_back(enemy);
}
void SwarmEnemy::RemoveSwarmEnemy(SwarmEnemy* enemy)
{
	auto it = std::find(s_swarm.begin(), s_swarm.end(), enemy);
	if (it != s_swarm.end())
	{
		std::iter_swap(it, s_swarm.end() - 1);
		s_swarm.pop_back();
	}
}

//object
SwarmEnemy::SwarmEnemy(EnemyManager* manager)
	:EnemyObject(manager)
	,m_forceScaler(1.0f)
	,m_avoidOtherDist(80.0f)
	,m_avoidObsDist(120.0f)
	,m_groupDist(200.0f)
{
	m_weights.aveVelocity = 0.5f;
	m_weights.toPlayer = 0.75f;
	m_weights.toCenter = 0.8f;
	m_weights.avoidOther = 1.2f;
	m_weights.avoidObs = 1.0f;

	AddSwarmEnemy(this);
}

SwarmEnemy::~SwarmEnemy()
{
	RemoveSwarmEnemy(this);
}

void SwarmEnemy::UpdateGameObject(float deltaTime)
{
	//���g�ƃO���[�v�ł�����̂̐�
	UINT groupSize = 0;
	//�O���[�v�̏d�S�ʒu
	Vector3 centerPos = Vector3::Zero;
	//�O���[�v�̕��ϕ����A���x
	Vector3 aveVelocity = Vector3::Zero;
	//�O���[�v/��Q���Ƃ̏Փ˂��������x�N�g��
	Vector3 avoidOther = Vector3::Zero;
	Vector3 avoidObs = Vector3::Zero;
	//�v���C���[�ւ̕���(�����قǋ����Ȃ�)
	Vector3 toTarget = Vector3::Zero;
	//�ŏI�I�ɉ������
	Vector3 addForce = Vector3::Zero;

	if (m_targetObj)
	{
		toTarget = m_targetObj->GetPosition() - this->GetPosition();
	}

	for (auto other : s_swarm)
	{
		if (this == other) continue;

		float distSq = Vector3::DistanceSq(this->GetPosition(), other->GetPosition());
		if (distSq <= m_groupDist * m_groupDist)
		{
			groupSize++;

			centerPos += other->GetPosition();
			aveVelocity += other->m_physicalMove->GetVelocity();
			if (distSq <= m_avoidOtherDist * m_avoidOtherDist)
			{
				auto temp = this->GetPosition() - other->GetPosition();
				if (distSq <= 100.0f)
				{
					temp *= 500.0f;
				}
				avoidOther += temp;
			}
		}
	}

	//��Q���������͂��v�Z
	for (auto obs : GAME.GetPhysWorld()->GetStaticBoxComps())
	{
		//AABB�Ƃ̋���
		auto distSq = obs->GetWorldBox().MinDistSq(this->GetPosition());
		if (distSq <= m_avoidObsDist * m_avoidObsDist)
		{
			auto vec = this->GetPosition() - obs->GetOwner()->GetPosition();
			vec.Normalize();
			avoidObs += vec * 25.0f;
		}
	}
	Vector3 fixVec;
	GAME.GetPhysWorld()->FixStaticBoxes(m_boxComp, fixVec);
	avoidObs += fixVec * 2000.0f;
	//DEBUG_LOG("avoid %f, %f, %f\n", avoidObs.x, avoidObs.y, avoidObs.z);

	//�ŏI�I�ɉ�����͂��v�Z
	if (groupSize > 0)
	{
		centerPos = centerPos / float(groupSize);
		aveVelocity = aveVelocity / float(groupSize);

		Vector3 toCenter = centerPos - this->GetPosition();

		addForce = 
			toCenter * m_weights.toCenter +
			aveVelocity * m_weights.aveVelocity +
			avoidOther * m_weights.avoidOther +
			avoidObs * m_weights.avoidObs +
			toTarget * m_weights.toPlayer;
		/*DEBUG_LOG("toCenter : %f, %f, %f\n", toCenter.x, toCenter.y, toCenter.z);
		DEBUG_LOG("aveVel : %f, %f, %f\n", aveVelocity.x, aveVelocity.y, aveVelocity.z);
		DEBUG_LOG("avoid : %f, %f, %f\n", avoidVec.x, avoidVec.y, avoidVec.z);*/
	}
	else
	{
		addForce = 
			toTarget * m_weights.toPlayer + 
			avoidObs * m_weights.avoidObs;
	}
	addForce *= m_forceScaler;

	m_physicalMove->AddStrikeForce(addForce);

	EnemyObject::UpdateGameObject(deltaTime);
}

void SwarmEnemy::OnCollision(const CollisionInfo& info)
{

}