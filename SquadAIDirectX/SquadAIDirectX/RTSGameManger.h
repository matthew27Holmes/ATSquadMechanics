#pragma once

#include "DXApp.h"
#include "model.h"


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

	//path finding
	void pathFind(int uniteID, int destinationId);

	bool AStar(Node unitLeaderNode, Node dest);
	bool isDestination(Node currNode, Node dest);
	vector<Node>createPath(Node curr,Node startNode);
	void addNeighbours(int row, int col, Node dest, Node parent);
	Node findLowestFScoringNode(Node dest);
	float findDistanceH(Node current, Node dest);

	bool isNodeInList(Node curr,vector<Node>List);
	vector<Node> removeNodeFromList(Node curr,vector<Node>List);
	bool isNodeVaild(Node currNode);
	Node findNodeInMap(int nodeID);
	int getGridSize() { return GridSize; }

	//unite order
	void floodFill(Node currStep, Unit Leader);

private: 
	int GridHeight, GridWidth, GridSize, NumberOfModles;
	vector<Unit> units;
	vector<Unit> selectedUnits; //possibly just path find for all units maybe just run a* for all units and check to make unite isnt on the node in the path 

	Node** gridMap;
	vector<Node> obsticles;

	vector<Node> openList;
	vector<Node> closedList;

	vector<Node> path;
	XMFLOAT2 destinationCor;
	Unit unitLeader;
	bool pathFound;
	int pathStep;
};