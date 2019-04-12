#include "Collider.h"



Collider::Collider(HINSTANCE hInstance) : DXApp(hInstance)
{}

Collider::~Collider()
{}

void Collider::Render(float dt)
{
	return;
}

void Collider::Update(float dt)
{
	return;
}

bool Collider::rayCollison(XMVECTOR rayDir, XMVECTOR rayOrgin)
{
	// rayDir is unit direction vector of ray
	XMVECTOR dirfrac = XMVectorSet(1.0f / XMVectorGetByIndex(rayDir, 0), 1.0f / XMVectorGetByIndex(rayDir, 1), 1.0f / XMVectorGetByIndex(rayDir, 2),1.0f);
	
	// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
	// r.org is origin of ray
	float t1 = (min.x - XMVectorGetByIndex(rayOrgin, 0))*XMVectorGetByIndex(dirfrac, 0);
	float t2 = (max.x - XMVectorGetByIndex(rayOrgin, 0))*XMVectorGetByIndex(dirfrac, 0);

	float t3 = (min.y - XMVectorGetByIndex(rayOrgin, 1))*XMVectorGetByIndex(dirfrac, 1);
	float t4 = (max.y - XMVectorGetByIndex(rayOrgin, 1))*XMVectorGetByIndex(dirfrac, 1);

	float t5 = (min.z - XMVectorGetByIndex(rayOrgin, 2))*XMVectorGetByIndex(dirfrac, 2);
	float t6 = (max.z - XMVectorGetByIndex(rayOrgin, 2))*XMVectorGetByIndex(dirfrac, 2);

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		t = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		t = tmax;
		return false;
	}

	t = tmin;// need to find object with the largest t 
	return true;
}

void Collider::setBox(XMFLOAT3 boxMin, XMFLOAT3 BoxMax)
{
	min = boxMin;
	max = BoxMax;
}
