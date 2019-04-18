#include "Collider.h"

Collider::Collider()
{}

Collider::~Collider()
{}

float Collider::rayCollison(XMVECTOR rayDir, XMVECTOR rayOrgin)
{
	/*// rayDir is unit direction vector of ray
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
		return numeric_limits<double>::infinity();
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		t = tmax;
		return numeric_limits<double>::infinity();
	}

	t = tmin;// need to find object with the largest t
	return t;*/

	XMFLOAT3 inv = XMFLOAT3(1 / XMVectorGetByIndex(rayDir, 0), 1 / XMVectorGetByIndex(rayDir, 1), 1 / XMVectorGetByIndex(rayDir, 2));

	XMFLOAT3 t0;
	t0.x = (min.x - XMVectorGetByIndex(rayOrgin, 0))/ XMVectorGetByIndex(rayDir, 0);
	t0.y = (min.y - XMVectorGetByIndex(rayOrgin, 1))/ XMVectorGetByIndex(rayDir, 1);
	t0.z = (min.z - XMVectorGetByIndex(rayOrgin, 2))/ XMVectorGetByIndex(rayDir, 2);
	
	XMFLOAT3 t1;
	t1.x = (max.x - XMVectorGetByIndex(rayOrgin, 0))/ XMVectorGetByIndex(rayDir, 0);
	t1.y = (max.y - XMVectorGetByIndex(rayOrgin, 1))/ XMVectorGetByIndex(rayDir, 1);
	t1.z = (max.z - XMVectorGetByIndex(rayOrgin, 2))/ XMVectorGetByIndex(rayDir, 2);

	XMFLOAT3 tmin;
	tmin.x = fminf(t0.x, t1.x);
	tmin.y = fminf(t0.y, t1.y);
	tmin.z = fminf(t0.z, t1.z);
	
	XMFLOAT3 tmax;
	tmax.x = fmaxf(t0.x, t1.x);
	tmax.y = fmaxf(t0.y, t1.y);
	tmax.z = fmaxf(t0.z, t1.z);

	if (maxComponent(tmin) <= minComponent(tmax))
	{
		return maxComponent(tmin);
	}
	else
	{
		return numeric_limits<double>::infinity();
	}
}

void Collider::setBox(XMFLOAT3 boxMin, XMFLOAT3 BoxMax)
{
	min = boxMin;
	max = BoxMax;
}

float Collider::maxComponent(XMFLOAT3 a)
{
	float max;
	if (a.x > a.y && a.x > a.z)
	{
		max = a.x;
	}
	else if(a.y > a.x && a.y > a.z)
	{
		max = a.y;
	}else if(a.z > a.x && a.z > a.y)
	{
		max = a.z;
	}
	return max;
}

float Collider::minComponent(XMFLOAT3 a)
{
	float min;
	if (a.x < a.y && a.x < a.z)
	{
		min = a.x;
	}
	else if (a.y < a.x && a.y < a.z)
	{
		min = a.y;
	}
	else if (a.z < a.x && a.z < a.y)
	{
		min = a.z;
	}
	return min;
}
