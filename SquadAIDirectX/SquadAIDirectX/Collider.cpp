#include "Collider.h"

Collider::Collider()
{}

Collider::~Collider()
{}

float Collider::rayCollison(XMVECTOR rayDir, XMVECTOR rayOrgin)
{
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
