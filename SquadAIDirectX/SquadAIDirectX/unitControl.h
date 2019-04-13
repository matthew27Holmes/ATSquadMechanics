#pragma once

#define X_MAX 100
#define X_STEP 01
#define Y_MAX 100
#define Y_STEP 01

#include "model.h"
#include <stack>
#include <Vector>
using namespace std;
struct Node
{
	int y;
	int x;
	int parentX;
	int parentY;
	float gCost;
	float hCost;
	float fCost;
};

inline bool operator < (const Node& lhs, const Node& rhs)
{//We need to overload "<" to put our struct into a set
	return lhs.fCost < rhs.fCost;
}


class unitControl : public model
{
public:

	unitControl(HINSTANCE hInstance);
	~unitControl() override;

	void Render(float dt) override;
	void Update(float dt) override;
	static bool isValid(float x,float y);
	static bool isDestination(float x, float y, Node dest);
	static float calculateH(float x, float y, Node dest);
	static vector<Node> aStar(Node player, Node dest);
	static vector<Node> makePath(array<array<Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> map, Node dest);
	void pathFind(int modelId, XMFLOAT3 modelPostion, XMFLOAT3 destination);

private: 

};

