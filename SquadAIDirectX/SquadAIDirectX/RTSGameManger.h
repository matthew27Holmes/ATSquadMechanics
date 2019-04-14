#pragma once

#define X_MAX 50
#define X_STEP 01
#define Y_MAX 50
#define Y_STEP 01

#include "DXApp.h"
#include "model.h"
#include <stack>
#include <Vector>
#include <array>
#include <map>

using namespace std;
struct Node
{
	int x;
	int z;
	int parentX;
	int parentZ;
	float gCost;
	float hCost;
	float fCost;
	bool IsWalkable;
};

struct Unit
{
	XMFLOAT3 postion;
	int unitID;
};

inline bool operator < (const Node& lhs, const Node& rhs)
{//We need to overload "<" to put our struct into a set
	return lhs.fCost < rhs.fCost;
}

class RTSGameManger : public model
{
public:

	RTSGameManger(HINSTANCE hInstance);
	~RTSGameManger() override;
	bool Init(ID3D11Device*);

	void Render(float dt, ID3D11DeviceContext*);
	void Update(float dt, ID3D11Device *);
	void createGrid();
	void createUnits();

	static bool isValid(float x,float y);
	static bool isDestination(float x, float y, Node dest);
	static float calculateH(float x, float y, Node dest);
	static vector<Node> aStar(Unit player, Node dest);
	static vector<Node> makePath(array<array<Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> map, Node dest);
	void pathFind(int modelId, XMFLOAT3 destination);


private: 
	int GridHeight, GridWidth, GridSize, NumberOfModles;
	vector<Unit> units;
	//std::map<int, std::string> stringLookupTable;
};