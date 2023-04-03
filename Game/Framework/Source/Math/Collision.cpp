#include "Collision.h"
#include <array>

LineSegment::LineSegment(const Vector3& start, const Vector3& end)
	:m_start(start)
	,m_end(end)
{}

Vector3 LineSegment::PointOnSegment(float t) const
{
	return m_start + (m_end - m_start) * t;
}

float LineSegment::MinDistSq(const Vector3& point) const
{
	Vector3 ab = m_end - m_start;
	Vector3 ba = -ab;
	Vector3 ac = point - m_start;
	Vector3 bc = point - m_end;

	if (Vector3::Dot(ab, ac) < 0)
	{
		return ac.LengthSq();
	}
	else if (Vector3::Dot(ba, bc) < 0)
	{
		return bc.LengthSq();
	}
	float scalar = Vector3::Dot(ac, ab) / ab.LengthSq(); 
	Vector3 p = scalar * ab; //c(point)����ab�֐����ɍ~�낹����W
	return (ac - p).LengthSq();
}

float LineSegment::MinDistSq(const LineSegment& s1, const LineSegment& s2)
{
	Vector3   u = s1.m_end - s1.m_start;
	Vector3   v = s2.m_end - s2.m_start;
	Vector3   w = s1.m_start - s2.m_start;
	float    a = Vector3::Dot(u, u);
	float    b = Vector3::Dot(u, v);
	float    c = Vector3::Dot(v, v);
	float    d = Vector3::Dot(u, w);
	float    e = Vector3::Dot(v, w);
	float    D = a * c - b * b; 
	float    sc, sN, sD = D; 
	float    tc, tN, tD = D; 

	if (math::NearZero(D)) { 
		sN = 0.0;       
		sD = 1.0;     
		tN = e;
		tD = c;
	}
	else {      
		sN = (b * e - c * d);
		tN = (a * e - b * d);
		if (sN < 0.0) { 
			sN = 0.0;
			tN = e;
			tD = c;
		}
		else if (sN > sD) {
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0) { 
		tN = 0.0;
		if (-d < 0.0)
			sN = 0.0;
		else if (-d > a)
			sN = sD;
		else {
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) { 
		tN = tD;
		if ((-d + b) < 0.0)
			sN = 0;
		else if ((-d + b) > a)
			sN = sD;
		else {
			sN = (-d + b);
			sD = a;
		}
	}

	sc = (math::NearZero(sN) ? 0.0f : sN / sD);
	tc = (math::NearZero(tN) ? 0.0f : tN / tD);

	Vector3 dP = w + (sc * u) - (tc * v);

	return dP.LengthSq();
}

Plane::Plane(const Vector3& normal, float d)
	:m_normal(normal)
	,m_d(d)
{}

Plane::Plane(const Vector3& a, const Vector3& b, const Vector3& c)
{
	Vector3 ab = b - a;
	Vector3 ac = c - a;
	//�O�ςŖ@���x�N�g���Z�o
	m_normal = Vector3::Cross(ab, ac);
	m_normal.Normalize();
	//d = -P dot normal
	m_d = -Vector3::Dot(a, m_normal);
}

float Plane::SignedDist(const Vector3& point) const
{
	return Vector3::Dot(point, m_normal) - m_d;
}

Sphere::Sphere(const Vector3& center, float radius)
	:m_position(center)
	, m_radius(radius)
{}

bool Sphere::Contains(const Vector3& point) const
{
	float distSq = (m_position - point).LengthSq();
	return distSq <= (m_radius * m_radius);
}

AABB::AABB(const Vector3& max, const Vector3& min)
	:m_max(max)
	,m_min(min)
{}

void AABB::UpdateMaxMin(const Vector3& point)
{
	m_min.x = std::fmin(m_min.x, point.x);
	m_min.y = std::fmin(m_min.y, point.y);
	m_min.z = std::fmin(m_min.z, point.z);

	m_max.x = std::fmax(m_max.x, point.x);
	m_max.y = std::fmax(m_max.y, point.y);
	m_max.z = std::fmax(m_max.z, point.z);
}

float AABB::MinDistSq(const Vector3& point) const
{
	// �e���ł̍�
	float dx = std::fmax(m_min.x - point.x, 0.0f);
	dx = std::fmax(dx, point.x - m_max.x);
	float dy = std::fmax(m_min.y - point.y, 0.0f);
	dy = std::fmax(dy, point.y - m_max.y);
	float dz = std::fmax(m_min.z - point.z, 0.0f);
	dz = std::fmax(dz, point.z - m_max.z);
	
	return dx * dx + dy * dy + dz * dz;
}

Vector3 AABB::MinDistVec(const Vector3& point) const
{
	// �e���ł̍�
	float dx1 = point.x - m_min.x;
	float dx2 = point.x - m_max.x;
	float dy1 = point.y - m_min.y;
	float dy2 = point.y - m_max.y;
	float dz1 = point.z - m_min.z;
	float dz2 = point.z - m_max.z;

	float dx = (std::abs(dx1) < std::abs(dx2)) ? dx1 : dx2;
	float dy = (std::abs(dy1) < std::abs(dy2)) ? dy1 : dy2;
	float dz = (std::abs(dz1) < std::abs(dz2)) ? dz1 : dz2;

	return Vector3(dx, dy, dz);
}

void AABB::Rotate(const Quaternion& q)
{
	//�{�b�N�X�̂W�_
	std::array<Vector3, 8> points;
	points[0] = m_min;
	points[1] = Vector3(m_max.x, m_min.y, m_min.z);
	points[2] = Vector3(m_min.x, m_max.y, m_min.z);
	points[3] = Vector3(m_min.x, m_min.y, m_max.z);

	points[4] = Vector3(m_min.x, m_max.y, m_max.z);
	points[5] = Vector3(m_max.x, m_min.y, m_max.z);
	points[6] = Vector3(m_max.x, m_max.y, m_min.z);
	points[7] = Vector3(m_max);

	//�����s�{�b�N�X���č\�z
	Vector3 p = Vector3::Rotate(points[0], q);
	m_min = p;
	m_max = p;
	for (size_t i = 1; i < points.size(); i++)
	{
		p = Vector3::Rotate(points[i], q);
		UpdateMaxMin(p);
	}
}

bool AABB::Contains(const Vector3& point) const
{
	bool isOut =
		point.x < m_min.x ||
		point.y < m_min.y ||
		point.z < m_min.z ||
		point.x > m_max.x ||
		point.y > m_max.y ||
		point.z > m_max.z;

	return !isOut;
}

Capsule::Capsule(const Vector3& start, const Vector3& end, float radius)
	:m_segment(start, end)
	,m_radius(radius)
{}

Vector3 Capsule::PointOnSegment(float t) const
{
	return m_segment.PointOnSegment(t);
}

bool Capsule::Contains(const Vector3& point) const
{
	float distSq = m_segment.MinDistSq(point);
	return distSq <= (m_radius * m_radius);
}

bool ConvexPolygon::Contains(const Vector2& point) const
{
	float sum = 0.0f;
	Vector2 a, b;
	for (auto i = 0; i < m_vertices.size() - 1; i++)
	{
		a = m_vertices[i] - point;
		a.Normalize();
		b = m_vertices[i + 1] - point;
		b.Normalize();
		//ab�Ԃ̊p�x�Z�o
		sum += std::acosf(Vector2::Dot(a, b));
	}
	a = m_vertices.back() - point;
	a.Normalize();
	b = m_vertices.front() - point;
	b.Normalize();
	sum += std::acosf(Vector2::Dot(a, b));
	//���v��360�x�ɋ߂��ꍇtrue
	return math::NearZero(sum - PI * 2);
}

bool Intersect(const Sphere& a, const Sphere& b)
{
	float dist = (a.m_position - b.m_position).LengthSq();
	float sumRadius = a.m_radius + b.m_radius;
	return dist <= (sumRadius * sumRadius);
}

bool Intersect(const AABB& a, const AABB& b)
{
	bool isOut =
		a.m_max.x < b.m_min.x ||
		b.m_max.x < a.m_min.x ||
		a.m_max.y < b.m_min.y ||
		b.m_max.y < a.m_min.y ||
		a.m_max.z < b.m_min.z ||
		b.m_max.z < a.m_min.z;

	return !isOut;
}

bool Intersect(const Capsule& a, const Capsule& b)
{
	float distSq = LineSegment::MinDistSq(a.m_segment, b.m_segment);
	float sumRadius = a.m_radius + b.m_radius;
	return distSq <= (sumRadius * sumRadius);
}

bool Intersect(const Sphere& sphere, const AABB& box)
{
	float minDistSq = box.MinDistSq(sphere.m_position);
	return minDistSq <= (sphere.m_radius * sphere.m_radius);
}

bool Intersect(const LineSegment& line, const Plane& plane, float& outT)
{
	//L(t)�En + d = 0�𖞂���t������ꍇ��������
	float denom = Vector3::Dot(line.m_end - line.m_start, plane.m_normal);
	//0�̏ꍇ�A���ʂƕ��s�ɂȂ�
	if (math::NearZero(denom))
	{
		//��������P�En - d == 0�𖞂����ꍇ�����̎n�_�����ʂɑ��݂��������邱�ƂɂȂ�
		if (math::NearZero(Vector3::Dot(line.m_start, plane.m_normal) - plane.m_d)) {
			outT = 0.0f;
			return true;
		}
		else return false;
	}
	
	//t = -Start�En - d / (End - Start)�En
	float numer = -Vector3::Dot(line.m_start, plane.m_normal) - plane.m_d;
	outT = numer / denom;
	if (outT >= 0.0f && outT <= 1.0f) return true;
	else return false;
}

bool Intersect(const LineSegment& line, const Sphere& sphere, float& outT)
{
	// ||L(t) - C|| = r �𖞂���t������ꍇ��������
	//X = Start - C
	//Y = End - Start
	//(Y�EY)t^2 + (2X�EY)t + (X�EX - r^2) = 0  (t�̂Q��������)
	Vector3 X = line.m_start - sphere.m_position;
	Vector3 Y = line.m_end - line.m_start;
	float a = Vector3::Dot(Y, Y);
	float b = 2.0f * Vector3::Dot(X, Y);
	float c = Vector3::Dot(X, X) - sphere.m_radius * sphere.m_radius;

	float disc = b * b - 4.0f * a * c;
	if (disc < 0)
	{
		//�������Ȃ�
		return false;
	}
	
	disc = std::sqrtf(disc);
	float tmin = (-b - disc) / (2 * a);
	float tmax = (-b + disc) / (2 * a);
	if (tmin <= 1.0f && tmin >= 0.0f) 
	{
		outT = tmin;
		return true;
	}
	else if (tmax <= 1.0f && tmax >= 0.0f)
	{
		outT = tmax;
		return true;
	}

	return false;
}

//line��AABB�̌�������ɗp����w���p�[�֐�
bool TestSidePlane(float start, float end, float d, const Vector3& norm, 
	std::vector<std::pair<float, Vector3>>& out)
{
	//t = -Start.xyz + negd.xyz / End.xyz - Start.xyz (���ʂ̌��������AABB�ɍ��킹�ĒP������������)

	float denom = end - start;
	if (math::NearZero(denom)) return false; //���ʂƕ��s�̏ꍇ
	else
	{
		float numer = -start + d;
		float t = numer / denom;
		if (t >= 0.0f && t <= 1.0f)
		{
			out.emplace_back(t, norm);
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Intersect(const LineSegment& line, const AABB& box, float& outT, Vector3& outNorm)
{
	//�������ʂƌ�������t�Ɩʂ̖@��
	std::vector<std::pair<float, Vector3>> tValues;
	//x���ʂ��e�X�g
	TestSidePlane(line.m_start.x, line.m_end.x, box.m_min.x, -Vector3::Right ,tValues); //XXX
	TestSidePlane(line.m_start.x, line.m_end.x, box.m_max.x, Vector3::Right, tValues);
	//y����
	TestSidePlane(line.m_start.y, line.m_end.y, box.m_min.y, -Vector3::Up, tValues);
	TestSidePlane(line.m_start.y, line.m_end.y, box.m_max.y, Vector3::Up, tValues);
	//z����
	TestSidePlane(line.m_start.z, line.m_end.z, box.m_min.z, -Vector3::Forward, tValues);
	TestSidePlane(line.m_start.z, line.m_end.z, box.m_max.z, Vector3::Forward, tValues);

	std::sort(tValues.begin(), tValues.end(), [](
		const std::pair<float, Vector3>& a,
		const std::pair<float, Vector3>& b)
		{return a.first < b.first; });
	
	Vector3 point;
	for (auto &t_n : tValues)
	{
		point = line.PointOnSegment(t_n.first);
		if (box.Contains(point))
		{
			outT = t_n.first;
			outNorm = t_n.second;
			return true;
		}
	}

	return false;
}

bool SweptSphere(const Sphere& P0, const Sphere& P1,
	const Sphere& Q0, const Sphere& Q1, float& outT)
{
	//P(t) = P0 + (P1 - P0)t
	//Q(t) = Q0 + (Q1 - Q0)t
	// ||P(t) - Q(t)|| = P.radius + Q.radius �𖞂����Ƃ���������
	//X = P0 - Q0
	//Y = (P1 - P0) - (Q1 - Q0)
	//(Y�EY)t^2 + (2X�EY)t + (X�EX - (P.radius + Q.radius)^2) �̓񎟕�����������
	Vector3 X = P0.m_position - Q0.m_position;
	Vector3 Y = (P1.m_position - P0.m_position) - (Q1.m_position - Q0.m_position);
	float a = Vector3::Dot(Y, Y);
	float b = 2.0f * Vector3::Dot(X, Y);
	float sumRadii = P0.m_radius + Q0.m_radius;
	float c = Vector3::Dot(X, X) - sumRadii * sumRadii;

	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f) return false; //�𖳂�
	else
	{
		disc = std::sqrtf(disc);
		outT = (-b - disc) / (2.0f * a); //�ŏ��Ɍ�����������
		if (outT >= 0.0f && outT <= 0.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
