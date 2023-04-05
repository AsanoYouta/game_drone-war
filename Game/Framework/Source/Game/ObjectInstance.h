#pragma once
#include "Source/DirectX/DirectX.h"
#include "GameObject.h"

//ゲームオブジェクトの外見(メッシュ)のインスタンスクラス
//ゲームオブジェクト１つにつき必ず１つ以上持つ
class ObjectInstance
{
public:
	ObjectInstance(GameObject* owner);
	~ObjectInstance();

	//Get/Set
	bool GetIsLive() const { return m_isLive; }
	const Vector3& GetPosition() const { return m_position; }
	float GetScale() const { return m_scale; }
	const Quaternion& GetRotation() const { return m_rotation; }
	const XMMATRIX& GetWorldTransform() const { return m_worldMat; }
	const XMFLOAT4& GetColor() const { return m_color; }
	void SetPosition(const Vector3& pos) { m_position = pos; m_owner->SetRecomputeWT(true); }
	void SetScale(float scale) { m_scale = scale; m_owner->SetRecomputeWT(true);}
	void SetRotation(const Quaternion& q) { m_rotation = q; m_owner->SetRecomputeWT(true);}
	//(r,g,b,a)
	void SetColor(const XMFLOAT4& col) { m_color = col; m_owner->SetRecomputeWT(true);}
	void SetLive(bool live) { m_isLive = live; }

	virtual void Update(float deltaTime);
	void ComputeWorldTransform();
	void SyncOwner();

protected:
	class GameObject* m_owner;

private:
	bool m_isLive;
	Vector3 m_position;
	Quaternion m_rotation;
	float m_scale;
	XMFLOAT4 m_color;
	XMMATRIX m_worldMat;
};

