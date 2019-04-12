#pragma once

#include "DXApp.h"
#include <vector>

using namespace std;

class Collider :public DXApp
{
public:
	Collider(HINSTANCE hInstance);
	~Collider();
	void Render(float dt) override;
	void Update(float dt)override;

	bool rayCollison(XMVECTOR rayDir, XMVECTOR rayOrgin);
	void setBox(XMFLOAT3 boxMin, XMFLOAT3 BoxMax);
private:
	XMFLOAT3 min, max;
	float t;

};

