#pragma once

#include "DXApp.h"
#include "Tile.h"
#include "SpacShip.h"


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
	bool selected;
	bool alive;
	int unitID;

	vector<Node> path;
	bool pathFound;
	int pathStep;
	Node dest;
	std::string team;
};

inline bool operator < (const Node& lhs, const Node& rhs)
{//We need to overload "<" to put our struct into a set
	return lhs.fCost < rhs.fCost;
}

class RTSGameManger
{
public:

	RTSGameManger(HINSTANCE hInstance);
	~RTSGameManger();
	bool Init(ID3D11Device*);

	void Render(float dt, ID3D11DeviceContext*);
	void Update(float dt, ID3D11Device *);
	void createGrid();
	void createUnits();

	//path finding
	Unit updateUnitePos(Unit unit);
	void findPath(int LeaderID, int destinationId);

	bool AStar(Node startNode, Node dest,int unitID);
	bool isDestination(Node currNode, Node dest);
	vector<Node>createPath(Node curr,Node startNode);
	void addNeighbours(int row, int col, Node dest, Node parent);
	Node findLowestFScoringNode();
	float findDistanceH(Node current, Node dest);

	bool isNodeInList(Node curr,vector<Node>List);
	vector<Node> removeNodeFromList(Node curr,vector<Node>List);
	bool isNodeVaild(Node currNode);
	Node findNodeInMap(int nodeID);
	int getGridSize() { return GridSize; }
	void selectUnite(int uniteId);
	void deSelecteAll();
	int getUniteByUnitID(int UniteID);
	void unitsAttacking(Unit currUnit);
	void killUnit(int uniteID);

	int getIndexCount();
	int getVertexCount();
	int getInstanceCount();

	int checkTileCollison(XMVECTOR rayDirc, XMVECTOR rayOrgin);
	int checkShipCollison(XMVECTOR rayDirc, XMVECTOR rayOrgin);

	//unite order
	Node floodFill(Node orginNode);

private: 
	int GridHeight, GridWidth, GridSize, NumberOfModles;
	vector<Unit> units;

	SpacShip* ships;
	Tile* Tiles;

	Node** gridMap;
	vector<Node> obsticles;
	vector<Node> openList;
	vector<Node> closedList;	
};