#pragma once
#include "Component.h"
#include "Source/Math/Collision.h"

//各Speedを設定し、それに伴ってSetPos,SetRotationを行う
//MoveComponentと違いベクトル(ワールド座標系)でスピードを指定し、慣性が働く
class MoveComponent2 : public Component
{
public:
	MoveComponent2(class GameObject* owner, int updateOrder = 10); //更新優先度高
	void Update(float deltaTime) override;
	void LimitVelocity();
	void LimitAccel();

	const Vector3& GetAccel() { return m_accel; }
	const Vector3& GetVelocity() { return m_velocity; }
	const Vector3& GetExceptionVel() { return m_exceptionVel; }
	const Vector3& GetMoveAmount() { return m_moveAmount; }
	float GetAngularSpeed() const { return m_angularSpeed; }
	void SetUseGravity(bool use) { m_useGravity = use; }
	void SetAccel(const Vector3& accel) { m_accel = accel; }
	void SetVelocity(const Vector3& velocity) { m_velocity = velocity; }
	void SetExceptionVel(const Vector3& velocity) { m_exceptionVel = velocity; }
	void SetAngularSpeed(float speed) { m_angularSpeed = speed; }
	void SetMaxSpeed(float speed) { m_maxSpeed = speed; }
	void SetMaxAccel(float accel) { m_maxAccel = accel; }
	void SetBrake(float brake) { m_brake = brake; }
	//速度制限を無視する瞬間加速度を追加
	void AddStrikeAccel(const Vector3& accel) { m_addAccel = accel; }

private:
	bool m_useGravity;
	Vector3 m_accel;
	Vector3 m_addAccel;
	Vector3 m_velocity;
	Vector3 m_exceptionVel;
	Vector3 m_gVelocity;
	Vector3 m_moveAmount; //前フレームの移動量
	float m_angularSpeed;
	float m_maxSpeed;
	float m_maxAccel;
	float m_brake;
};


