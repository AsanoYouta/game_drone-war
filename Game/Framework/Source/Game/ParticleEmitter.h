#pragma once
#include "GameObject.h"
#include <random>

class ParticleEmitter : public GameObject
{
public:
	ParticleEmitter(UINT texNum = 1);

	void SetMaterial(Material mat);
	//�p�[�e�B�N���̃e�N�X�`����ݒ�(1:���߉~�@2:�񓧉߉~)
	void SetTexture(UINT texNum);

	//����Ƀp�[�e�B�N������o����
	void GenerateScatter(
		UINT gNumSq,    //��������p�[�e�B�N���̓�捪��
		float force,    //�p�[�e�B�N���ɂ������(velocity�̃X�J���[)
		float lifespan, //���݂���b��
		float mulVec,   //���t���[��velocity�ɂ�����X�J���[
		float addScale, //���t���[���X�P�[���ɉ��Z���鐔
		XMFLOAT4 addCol //���t���[�����Z����F
	);
	//�����_���ȗ͂ŋ���Ƀp�[�e�B�N������o����
	void GenerateRandScatter(UINT gNumSq,float maxForce, float lifespan,
		float mulVec, float addScale, XMFLOAT4 addCol);
	//n�b�ԃp�[�e�B�N������o����
	void EmitParticle(
		float emitTime,  //���o�������鎞��
		UINT gNum,		 //������o����p�[�e�B�N����
		float posRange,  //���o�ʒu�̃Y���͈̔�
		float gAccel,    //�d�͉����x
		Vector3 velocity,//�������x
		float velRange,  //�������x�ɉ�����x�N�g���e�����͈̔�
		float lifespan,  //���݂���b��
		float mulVec,    //���t���[��velocity�ɂ�����X�J���[
		float addScale,  //���t���[���X�P�[���ɉ��Z���鐔
		XMFLOAT4 col,    //�����F
		XMFLOAT4 addCol  //���t���[�����Z����F
	);

	void UpdateGameObject(float deltaTime) override;
	void Destroy();

protected:
	class MeshComponent* m_particleModel;
	float m_emitTime;
	bool m_emitted = false;

private:
	std::mt19937 m_mt;
	//�p�[�e�B�N���p�����[�^
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

