#pragma once
#include "EnemyObject.h"

class SwarmEnemy : public EnemyObject
{
public:
	//�������(�����x�x�N�g��)�̏d��
	struct Weights
	{
		float toCenter;
		float aveVelocity;
		float toPlayer;
		float avoidOther;
		float avoidObs;
	};

	SwarmEnemy(EnemyManager* manager = nullptr);
	~SwarmEnemy();

	void UpdateGameObject(float deltaTime) override;
	//�Փˎ��ɌĂяo�����
	void OnCollision(const CollisionInfo& info) override;

private:
	float m_groupDist;
	float m_avoidOtherDist;
	float m_avoidObsDist;
	float m_forceScaler;
	Weights m_weights;

//Static
private:
	static std::vector<SwarmEnemy*> s_swarm;
	static void AddSwarmEnemy(SwarmEnemy* enemy);
	static void RemoveSwarmEnemy(SwarmEnemy* enemy);
};

