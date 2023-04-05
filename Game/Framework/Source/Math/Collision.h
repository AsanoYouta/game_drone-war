#pragma once
#include "mymath.h"

struct LineSegment
{
	LineSegment(const Vector3& start, const Vector3& end);

	//�������t�{�����_
	Vector3 PointOnSegment(float t) const;
	//�����Ɠ_�̊Ԃōł��߂�������2����Z�o(�����ȕ��������Z�������)
	float MinDistSq(const Vector3& point) const;
	static float MinDistSq(const LineSegment& s1, const LineSegment& s2);

	Vector3 m_start;
	Vector3 m_end;
};

struct Plane
{
	Plane(const Vector3& normal, float d);
	//3�_�ォ�畽�ʂ��\�z
	Plane(const Vector3& a, const Vector3& b, const Vector3& c);

	//point�Ƃ̕����t���ŒZ�������Z�o(�����͖@�������ɏ���)
	float SignedDist(const Vector3& point) const;

	Vector3 m_normal;
	float m_d; //���_����@�������ɕ��ʂ܂ŐL�΂�������(�ŒZ����)
	//���ʂ̕����� : P�En + d = 0
};

struct Sphere
{
	Sphere(const Vector3& center, float radius);

	bool Contains(const Vector3& point) const;

	Vector3 m_position;
	float m_radius;
};

struct AABB
{
	AABB(const Vector3& max, const Vector3& min);

	//��̒��_�ɂ��čő�ŏ��l���r�A�X�V����(�S�Ă̒��_�ɂ��Ď��s����K�v������)
	void UpdateMaxMin(const Vector3& point);
	void Rotate(const Quaternion& q);
	bool Contains(const Vector3& point) const;
	//point�ƃ{�b�N�X�̍ŒZ����
	float MinDistSq(const Vector3& point) const;
	//point�֌������x�N�g��
	Vector3 MinDistVec(const Vector3& point) const;

	Vector3 m_max;
	Vector3 m_min;
	//�O���[�o�����ƕ��s�ł��邽�߂Q�̍ő�ŏ��x�N�g���ŕ\����
};

struct Capsule  //��(���a)��������
{
	Capsule(const Vector3& start, const Vector3& end, float radius);
	Vector3 PointOnSegment(float t) const;
	bool Contains(const Vector3& point) const;

	LineSegment m_segment;
	float m_radius;
};

//�o�E���f�B���O�{�����[���Ԃ̌�������
bool Intersect(const Sphere& a, const Sphere& b);
bool Intersect(const AABB& a, const AABB& b);
bool Intersect(const Capsule& a, const Capsule& b);
bool Intersect(const Sphere& sphere, const AABB& box);
//�����Ƃ̌�������(ray)
bool Intersect(const LineSegment& line, const Sphere& sphere, float& outT);
bool Intersect(const LineSegment& line, const Plane& plane, float& outT);
bool Intersect(const LineSegment& line, const AABB& box, float& outT, Vector3& outNorm);
//outT����LineSegment::PointOnSegment(float t)��p���Č��������_���W�𓾂���

//CCD
bool SweptSphere(const Sphere& P0, const Sphere& P1,
	const Sphere& Q0, const Sphere& Q1, float& t);

