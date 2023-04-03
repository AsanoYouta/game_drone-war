#pragma once
#include "EnemyObject.h"

class SwarmEnemy : public EnemyObject
{
public:
	//かける力(加速度ベクトル)の重み
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
	//衝突時に呼び出される
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

