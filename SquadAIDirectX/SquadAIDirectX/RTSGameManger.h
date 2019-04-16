#pragma once

#include "DXApp.h"
#include "model.h"
#include <Vector>
#include <array>
#include <algorithm>

using namespace std;
struct Node
{
	XMFLOAT3 position;
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
	XMFLOAT3 position;
	XMFLOAT2 cordinates;
	bool Leader;
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
	//find unit 
	//find node 

	//unite order
	XMFLOAT3 cohesion(Unit currUnit);
	XMFLOAT3 seperation(Unit currUnit);
	//path finding
	void pathFind(int modelId, XMFLOAT3 destination);

	bool AStar(Node unitLeaderNode, Node dest);
	vector<Node>createPath(Node curr,Node startNode);
	void addNeighbours(int row, int col, Node dest, Node parent);
	void removeNodeFromOpenList(Node curr);
	Node findLowestFScoringNode(Node dest);
	float findDistanceH(Node current, Node dest);
	bool isNodeInList(Node curr,vector<Node>List);
private: 
	int GridHeight, GridWidth, GridSize, NumberOfModles;
	vector<Unit> units;
	Node** gridMap;
	vector<Node> openList;
	vector<Node> closedList;

	vector<Node> path;
	XMFLOAT2 destinationCor;
	Unit unitLeader;
	bool pathFound;
	int pathStep;
};