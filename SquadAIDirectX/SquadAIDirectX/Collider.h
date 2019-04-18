#pragma once

#include "DXApp.h"

class Collider
{
public:
	Collider();
	~Collider();

	float rayCollison(XMVECTOR rayDir, XMVECTOR rayOrgin);
	void setBox(XMFLOAT3 boxMin, XMFLOAT3 BoxMax);
	float maxComponent(XMFLOAT3 a);
	float minComponent(XMFLOAT3 a);

	float getT() { return t; }

private:
	XMFLOAT3 min, max;
	float t;

};

