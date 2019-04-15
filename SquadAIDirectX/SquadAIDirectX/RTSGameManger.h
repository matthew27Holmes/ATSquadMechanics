#pragma once

#include "DXApp.h"
#include "model.h"
#include <Vector>
#include <array>
#include <algorithm>

using namespace std;
struct Node
{
	XMFLOAT3 postion;
	int id;
	XMFLOAT2 cordinates;
	bool IsWalkable;
	XMFLOAT2 parentCordinates;
	float gCost;
	float hCost;
	float fCost;
};

struct Unit
{
	XMFLOAT3 postion;
	XMFLOAT2 cordinates;
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

	void pathFind(int modelId, XMFLOAT3 destination);
	Node findUnitLeaderNode(Unit unitLeader);

	vector<Node>AStar(Node unitLeaderNode, Node dest);
	void addNeighbours(int row, int col);
	Node findLowestFScoringNode(Node dest);
	float findDistanceH(Node current, Node dest);
	bool isNodeInList(Node curr,vector<Node>List);
private: 
	int GridHeight, GridWidth, GridSize, NumberOfModles;
	vector<Unit> units;
	Node** gridMap;
	vector<Node> openList;
	vector<Node> closedList;
	XMFLOAT2 destinationTemp;
};