#pragma once
#include "Source/Math/mymath.h"
#include "Source/Math/Collision.h"

struct CollisionInfo
{
	Vector3 collidePoint;
	Vector3 normal;
	class BoxComponent* boxComp;
	class GameObject* GameObj;
};

class PhysWorld
{
public:
	PhysWorld();

	bool SegmentCast(const LineSegment& line, CollisionInfo& outColl, BoxComponent* exception = nullptr);
	bool SweptAllBoxes(class SphereComponent* sphereComp, CollisionInfo& outColl);
	bool FixAllBoxes(class BoxComponent* boxComp, Vector3& fixVec);
	bool FixStaticBoxes(class BoxComponent* boxComp, Vector3& fixVec);

	//ワールドにコリジョンコンポーネントを追加/削除
	void AddBoxComp(class BoxComponent* box);
	void RemoveBoxComp(class BoxComponent* box);
	void AddSphereComp(class SphereComponent* sphere);
	void RemoveSphereComp(class SphereComponent* sphere);

	const std::vector<class BoxComponent*>& GetBoxComps() { return m_boxComps; }
	const std::vector<class BoxComponent*>& GetStaticBoxComps() { return m_staticBoxComps; }
	const std::vector<class SphereComponent*>& GetSphereComps() { return m_sphereComps; }

private:
	std::vector<class BoxComponent* > m_boxComps;
	std::vector<class BoxComponent* > m_staticBoxComps;
	std::vector<class SphereComponent*> m_sphereComps;
};

