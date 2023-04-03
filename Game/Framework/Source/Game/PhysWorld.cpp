#include "PhysWorld.h"
#include "BoxComponent.h"
#include "SphereComponent.h"
#include "GameObject.h"

PhysWorld::PhysWorld()
{

}

void PhysWorld::AddBoxComp(BoxComponent* box)
{
	m_boxComps.emplace_back(box);
	if (box->GetOwner()->GetType() == GameObject::Static)
	{
		m_staticBoxComps.emplace_back(box);
	}
}
void PhysWorld::AddSphereComp(SphereComponent* sphere)
{
	m_sphereComps.emplace_back(sphere);
}

void PhysWorld::RemoveBoxComp(BoxComponent* box)
{
	auto it = std::find(m_boxComps.begin(), m_boxComps.end(), box);
	if (it != m_boxComps.end())
	{
		std::iter_swap(it, m_boxComps.end() - 1);
		m_boxComps.pop_back();
	}
	it = std::find(m_staticBoxComps.begin(), m_staticBoxComps.end(), box);
	if (it != m_staticBoxComps.end())
	{
		std::iter_swap(it, m_staticBoxComps.end() - 1);
		m_staticBoxComps.pop_back();
	}
}
void PhysWorld::RemoveSphereComp(SphereComponent* sphere)
{
	auto it = std::find(m_sphereComps.begin(), m_sphereComps.end(), sphere);
	if (it != m_sphereComps.end())
	{
		std::iter_swap(it, m_sphereComps.end() - 1);
		m_sphereComps.pop_back();
	}
}

//��������������
bool PhysWorld::SegmentCast(const LineSegment& line, CollisionInfo& outColl, BoxComponent* exception)
{
	bool collided = false;
	float closestT = math::Inifinity;
	Vector3 norm;

	//�������ꂽ�S�Ă�boxComponent�ƃe�X�g(�ڐG�����ł��߂�box��I��)
	for (auto box : m_boxComps)
	{
		//��O�{�b�N�X�͖���
		if (exception && exception == box) continue;

		float t;
		if (Intersect(line, box->GetWorldBox(), t, norm))
		{
			if (t < closestT)
			{
				outColl.boxComp = box;
				outColl.collidePoint = line.PointOnSegment(t);
				outColl.normal = norm;
				outColl.GameObj = box->GetOwner();
				closestT = t;
				collided = true;
			}
		}
	}

	return collided;
}

bool PhysWorld::SweptAllBoxes(SphereComponent* sphereComp, CollisionInfo& outColl)
{
	//sphere�ʒu���X�V���Ă���
	sphereComp->GetOwner()->ComputeWorldTransform();

	Sphere P0 = sphereComp->GetPreWorldSphere();
	Sphere P1 = sphereComp->GetWorldSphere();
	Vector3 p0_p1 = Vector3::Normalized(P1.m_position - P0.m_position);
	//���a�����O�������Ԃ̃��C��
	auto l = LineSegment(P0.m_position + (p0_p1 * P0.m_radius), P1.m_position + (p0_p1 * P1.m_radius));
	float closestT = math::Inifinity;
	bool collided = false;

	for (auto b : m_boxComps)
	{
		float t;
		Vector3 n;
		if (Intersect(l, b->GetWorldBox(), t, n))
		{
			if (t < closestT)
			{
				outColl.boxComp = b;
				outColl.collidePoint = l.PointOnSegment(t);
				outColl.normal = n;
				outColl.GameObj = b->GetOwner();
				closestT = t;
				collided = true;
			}
		}
	}

	return collided;
}

bool PhysWorld::FixAllBoxes(BoxComponent* boxComp, Vector3& fixVec)
{
	//���g��boxComponent�̏�Ԃ��X�V���Ă���
	boxComp->GetOwner()->ComputeWorldTransform();

	Vector3 pos = boxComp->GetOwner()->GetPosition();
	const AABB& thisBox = boxComp->GetWorldBox();

	bool contact = false;
	fixVec = Vector3::Zero;

	for (auto box : m_boxComps)
	{
		if (box == boxComp) continue;

		//�ʒu�C������
		const AABB& otherBox = box->GetWorldBox();
		if (Intersect(otherBox, thisBox))
		{
			//�ŏ��̍����ڐG�����{�b�N�X�Ԃ̋����Ƃ������ƂɂȂ�(�ő�A�ŏ��ʒu���m�̐ڐG�͂��肦�Ȃ�)
			float dx1 = otherBox.m_max.x - thisBox.m_min.x;
			float dx2 = otherBox.m_min.x - thisBox.m_max.x;
			float dy1 = otherBox.m_max.y - thisBox.m_min.y;
			float dy2 = otherBox.m_min.y - thisBox.m_max.y;
			float dz1 = otherBox.m_max.z - thisBox.m_min.z;
			float dz2 = otherBox.m_min.z - thisBox.m_max.z;

			float dx = (std::abs(dx1) < std::abs(dx2)) ? dx1 : dx2;
			float dy = (std::abs(dy1) < std::abs(dy2)) ? dy1 : dy2;
			float dz = (std::abs(dz1) < std::abs(dz2)) ? dz1 : dz2;

			if (std::abs(dx) <= std::abs(dy) && std::abs(dx) <= std::abs(dz))
			{
				pos.x += dx;
				fixVec.x += dx;
			}
			else if (std::abs(dy) <= std::abs(dx) && std::abs(dy) <= std::abs(dz))
			{
				pos.y += dy;
				fixVec.y += dy;
			}
			else
			{
				pos.z += dz;
				fixVec.z += dz;
			}

			boxComp->GetOwner()->SetPosition(pos);
			boxComp->OnUpdateWorldTransform();

			contact = true;
		}
	}
	return contact;
}

bool PhysWorld::FixStaticBoxes(class BoxComponent* boxComp, Vector3& fixVec)
{
	//���g��boxComponent�̏�Ԃ��X�V���Ă���
	boxComp->GetOwner()->ComputeWorldTransform();

	Vector3 pos = boxComp->GetOwner()->GetPosition();
	const AABB& thisBox = boxComp->GetWorldBox();

	bool contact = false;
	fixVec = Vector3::Zero;

	for (auto box : m_staticBoxComps)
	{
		//���g�̃{�b�N�X�͖���
		if (box == boxComp) 
			continue;

		//�ʒu�C������
		const AABB& otherBox = box->GetWorldBox();
		if (Intersect(otherBox, thisBox))
		{
			//�ŏ��̍����ڐG�����{�b�N�X�Ԃ̋����Ƃ������ƂɂȂ�(�ő�A�ŏ��ʒu���m�̐ڐG�͂��肦�Ȃ�)
			float dx1 = otherBox.m_max.x - thisBox.m_min.x;
			float dx2 = otherBox.m_min.x - thisBox.m_max.x;
			float dy1 = otherBox.m_max.y - thisBox.m_min.y;
			float dy2 = otherBox.m_min.y - thisBox.m_max.y;
			float dz1 = otherBox.m_max.z - thisBox.m_min.z;
			float dz2 = otherBox.m_min.z - thisBox.m_max.z;

			float dx = (std::abs(dx1) < std::abs(dx2)) ? dx1 : dx2;
			float dy = (std::abs(dy1) < std::abs(dy2)) ? dy1 : dy2;
			float dz = (std::abs(dz1) < std::abs(dz2)) ? dz1 : dz2;

			if (std::abs(dx) <= std::abs(dy) && std::abs(dx) <= std::abs(dz))
			{
				pos.x += dx;
				fixVec.x += dx;
			}
			else if (std::abs(dy) <= std::abs(dx) && std::abs(dy) <= std::abs(dz))
			{
				pos.y += dy;
				fixVec.y += dy;
			}
			else
			{
				pos.z += dz;
				fixVec.z += dz;
			}

			boxComp->GetOwner()->SetPosition(pos);
			boxComp->OnUpdateWorldTransform();
			contact = true;
		}
	}
	return contact;
}
