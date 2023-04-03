#pragma once
#include "GameObject.h"
#include <random>

class ParticleEmitter : public GameObject
{
public:
	ParticleEmitter(UINT texNum = 1);

	void SetMaterial(Material mat);
	//パーティクルのテクスチャを設定(1:透過円　2:非透過円)
	void SetTexture(UINT texNum);

	//球状にパーティクルを放出する
	void GenerateScatter(
		UINT gNumSq,    //生成するパーティクルの二乗根数
		float force,    //パーティクルにかける力(velocityのスカラー)
		float lifespan, //存在する秒数
		float mulVec,   //毎フレームvelocityにかけるスカラー
		float addScale, //毎フレームスケールに加算する数
		XMFLOAT4 addCol //毎フレーム加算する色
	);
	//ランダムな力で球状にパーティクルを放出する
	void GenerateRandScatter(UINT gNumSq,float maxForce, float lifespan,
		float mulVec, float addScale, XMFLOAT4 addCol);
	//n秒間パーティクルを放出する
	void EmitParticle(
		float emitTime,  //放出し続ける時間
		UINT gNum,		 //毎回放出するパーティクル数
		float posRange,  //放出位置のズレの範囲
		float gAccel,    //重力加速度
		Vector3 velocity,//初期速度
		float velRange,  //初期速度に加えるベクトル各成分の範囲
		float lifespan,  //存在する秒数
		float mulVec,    //毎フレームvelocityにかけるスカラー
		float addScale,  //毎フレームスケールに加算する数
		XMFLOAT4 col,    //初期色
		XMFLOAT4 addCol  //毎フレーム加算する色
	);

	void UpdateGameObject(float deltaTime) override;
	void Destroy();

protected:
	class MeshComponent* m_particleModel;
	float m_emitTime;
	bool m_emitted = false;

private:
	std::mt19937 m_mt;
	//パーティクルパラメータ
	Vector3 m_velocity;
	UINT m_gNum;
	float m_velRange;
	float m_posRange;
	float m_lifespan;
	float m_mulVec;
	float m_addScale;
	float m_gAccel;
	XMFLOAT4 m_color;
	XMFLOAT4 m_addCol;
};

