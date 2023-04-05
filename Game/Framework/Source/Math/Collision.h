#pragma once
#include "mymath.h"

struct LineSegment
{
	LineSegment(const Vector3& start, const Vector3& end);

	//直線上のt倍した点
	Vector3 PointOnSegment(float t) const;
	//線分と点の間で最も近い距離の2乗を算出(高価な平方根演算を避ける)
	float MinDistSq(const Vector3& point) const;
	static float MinDistSq(const LineSegment& s1, const LineSegment& s2);

	Vector3 m_start;
	Vector3 m_end;
};

struct Plane
{
	Plane(const Vector3& normal, float d);
	//3点上から平面を構築
	Plane(const Vector3& a, const Vector3& b, const Vector3& c);

	//pointとの符号付き最短距離を算出(符号は法線方向に準拠)
	float SignedDist(const Vector3& point) const;

	Vector3 m_normal;
	float m_d; //原点から法線方向に平面まで伸ばした距離(最短距離)
	//平面の方程式 : P・n + d = 0
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

	//一つの頂点について最大最小値を比較、更新する(全ての頂点について実行する必要がある)
	void UpdateMaxMin(const Vector3& point);
	void Rotate(const Quaternion& q);
	bool Contains(const Vector3& point) const;
	//pointとボックスの最短距離
	float MinDistSq(const Vector3& point) const;
	//pointへ向いたベクトル
	Vector3 MinDistVec(const Vector3& point) const;

	Vector3 m_max;
	Vector3 m_min;
	//グローバル軸と平行であるため２つの最大最小ベクトルで表せる
};

struct Capsule  //球(半径)を持つ線分
{
	Capsule(const Vector3& start, const Vector3& end, float radius);
	Vector3 PointOnSegment(float t) const;
	bool Contains(const Vector3& point) const;

	LineSegment m_segment;
	float m_radius;
};

//バウンディングボリューム間の交差判定
bool Intersect(const Sphere& a, const Sphere& b);
bool Intersect(const AABB& a, const AABB& b);
bool Intersect(const Capsule& a, const Capsule& b);
bool Intersect(const Sphere& sphere, const AABB& box);
//線分との交差判定(ray)
bool Intersect(const LineSegment& line, const Sphere& sphere, float& outT);
bool Intersect(const LineSegment& line, const Plane& plane, float& outT);
bool Intersect(const LineSegment& line, const AABB& box, float& outT, Vector3& outNorm);
//outTからLineSegment::PointOnSegment(float t)を用いて交差した点座標を得られる

//CCD
bool SweptSphere(const Sphere& P0, const Sphere& P1,
	const Sphere& Q0, const Sphere& Q1, float& t);

