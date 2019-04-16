#include "RTSGameManger.h"

RTSGameManger::RTSGameManger(HINSTANCE hInstance) : model(hInstance)
{
	NumberOfModles = 4;
	GridHeight = 50;
	GridWidth = 50;
	GridSize = GridWidth * GridHeight;
	gridMap = new Node* [GridHeight];
	for (int i = 0; i < GridHeight; ++i)
	{
		gridMap[i] = new Node[GridWidth];
	}
	pathFound = false;
	pathStep = 0;
}

RTSGameManger::~RTSGameManger()
{
	for (int i = 0; i < GridHeight; ++i) {
		delete[] gridMap[i];
	}
	delete[] gridMap;
}

bool RTSGameManger::Init(ID3D11Device *device)
{
	if (!model::Init(device))
	{
		OutputDebugString("Could not initialize the model object.");
		return false;
	}	
	//should have seperate instance buffers for grid and units
	model::initializeInstance(GridSize, NumberOfModles);
	createGrid();
	createUnits();
	return true;
}

void RTSGameManger::Render(float dt, ID3D11DeviceContext* deviceContext)
{
	model::RenderBuffers(deviceContext);
	return;
}

void RTSGameManger::Update(float dt, ID3D11Device *device)
{
	model::Update(dt);
	model::updateInstancesBuffer(device);

	if (pathFound)
	{
		XMFLOAT3 currentPos = getInstancePos(unitLeader.unitID);
		XMFLOAT3 goalPos = XMFLOAT3(path[pathStep].position.x, unitLeader.position.y, path[pathStep].position.z);

		moveTo(unitLeader.unitID, goalPos);

		if(currentPos.x == path[pathStep].position.x && currentPos.z == path[pathStep].position.z)
		{
			if (pathStep <= path.size()-2)
			{
				pathStep++;
			}
			else
			{
				pathStep = path.size() - 1;
				pathFound = false;
			}
		}
		//updateInstancePos(UnitLeader.unitID, node.postion.x, UnitLeader.postion.y, node.postion.z);
	}

	return;
}

void RTSGameManger::createGrid()
{
	int modelID = 0;
	for (int k = 0; k < GridHeight;k++)
	{
		for (int i = 0; i < GridWidth;i++)
		{
			XMFLOAT3 scale = { 1.0f, 0.1f, 1.0f };
			XMFLOAT3 rotaion = { 0.0f, 0.0f, 0.0f };
			XMFLOAT3 postion = { (float)i * 2, 1.0f, (float)k * 2 };
			if (i == GridWidth - 1 && k == GridHeight-1)
			{
				postion.y = 2.0f;// far corner
				destinationCor.x = i;
				destinationCor.y = k;
			}
			model::addInstance(modelID, postion, scale, rotaion);
			Node nwNode;
			nwNode.position = postion;
			nwNode.id = modelID;
			nwNode.cordinates.x = i;
			nwNode.cordinates.y = k;
			nwNode.IsWalkable = true;
			gridMap[k][i] = nwNode;
			modelID++;
		}
	}
}

void RTSGameManger::createUnits()
{
	float posModX = 0.0f, posModZ = 0.0f;
	//create units 
	Unit nwUnit;
	units.assign(NumberOfModles, nwUnit);
	int modelID = GridSize - 1;// -1 zero index// instance buffer starts at the end of grid 
	for (int u = 0; u < NumberOfModles; u++)
	{
		modelID++;
		posModX += 1.0f;

		XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
		XMFLOAT3 rotaion = { 0.0f, 0.0f, 0.0f };
		posModX += 1.0f;
		nwUnit.position = { posModX * 2, 2.0f, posModZ * 2 };;
		model::addInstance(modelID, nwUnit.position, scale, rotaion);

		nwUnit.unitID = modelID;
		nwUnit.cordinates.x = nwUnit.position.x;
		nwUnit.cordinates.y = nwUnit.position.z;

		units[u] = nwUnit;

		if (posModX >= 2)
		{
			posModX = 0.0f;
			posModZ += 1.0f;
		}
	}

}

#pragma region unitControl

//cohseion 
XMFLOAT3 RTSGameManger::cohesion(Unit currUnit)
{
	XMFLOAT3 centerMass;
	for (Unit unit : units)
	{
		if (currUnit.unitID != unit.unitID)
		{
			centerMass.x =+ unit.position.x;
			centerMass.y =+ unit.position.y;
			centerMass.z =+ unit.position.z;
		}
	}
	centerMass.x /= units.size()-1;
	centerMass.y /= units.size()-1;
	centerMass.z /= units.size()-1;

	centerMass.x = (centerMass.x - currUnit.position.x) / 100;
	centerMass.y = (centerMass.y - currUnit.position.y) / 100;
	centerMass.z = (centerMass.z - currUnit.position.z) / 100;
	return centerMass;
}


//seperation
XMFLOAT3 RTSGameManger::seperation(Unit currUnit)
{
	/*PROCEDURE rule2(boid bJ)

	Vector c = 0;

	FOR EACH BOID b
	IF b != bJ THEN
	c = c - (b.position - bJ.position)
	END IF
	END IF
	END

	RETURN c

	END PROCEDURE*/

	XMFLOAT3 seperation;
	for (Unit unit : units)
	{
		if (currUnit.unitID != unit.unitID)
		{
			XMFLOAT3 distances;
			distances.x = unit.position.x - currUnit.position.x;
			distances.y = unit.position.y - currUnit.position.y;
			distances.z = unit.position.z - currUnit.position.z;
			if (distances < 100)
			{
				seperation.x =- (unit.position.x - currUnit.position.x);
				seperation.y =- (unit.position.y - currUnit.position.y);
				seperation.z =-(unit.position.z - currUnit.position.z);

			}
		}
	}
	return seperation;
}



#pragma endregion


#pragma region PathFinding

bool RTSGameManger::AStar(Node unitLeaderNode, Node dest)
{
	openList.push_back(unitLeaderNode);
	
	//node with lowest h
	while (!openList.empty())
	{
		Node currentNode = findLowestFScoringNode(dest);

		if (currentNode.id == dest.id)
		{
			// add all parents form the closed list 
			path = createPath(currentNode, unitLeaderNode);
			return true;
		}
		else
		{
			closedList.push_back(currentNode);
			removeNodeFromOpenList(currentNode);
			addNeighbours(currentNode.cordinates.x, currentNode.cordinates.y,dest, currentNode);
		}
	}
	return false;
}

vector<Node> RTSGameManger::createPath(Node curr, Node startNode)
{
	// backtrack through all parent nodes until you reach the start
	vector<Node> temp;
	Node parnetNode = gridMap[(int)curr.cordinates.y][(int)curr.cordinates.x];
	temp.push_back(parnetNode);
	while (parnetNode.id != startNode.id)
	{
		parnetNode = gridMap[(int)parnetNode.parentCordinates.y][(int)parnetNode.parentCordinates.x];
		temp.push_back(parnetNode);
	}
	reverse(temp.begin(), temp.end());
	return temp;
}

void RTSGameManger::addNeighbours(int x, int y, Node dest,Node parent)
{
	if (y > 1 && y < GridHeight -1
		&& x > 1 && x < GridWidth - 1)
	{
		Node neighbour;

		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				neighbour = gridMap[y + i][x + j];
				neighbour.gCost = 1.0f;

				// diagnoal
				if (abs(i) == abs(j))
				{
					neighbour.gCost = 1.4f;
				}
				neighbour.hCost = findDistanceH(neighbour, dest); // find distance from this node to destination
				neighbour.fCost = neighbour.hCost + neighbour.gCost;
				neighbour.parentCordinates = parent.cordinates;

				if (!isNodeInList(neighbour, openList))
				{
					if (!isNodeInList(neighbour, closedList))
					{
						if (neighbour.IsWalkable)
						{
							gridMap[y + i][x + j] = neighbour;
							openList.push_back(neighbour);
						}
					}
				}
			}
		}
	}
}

void RTSGameManger::removeNodeFromOpenList(Node curr)
{
	for (int i = 0; i < openList.size(); i++)
	{
		Node temp = openList[i];
		if (temp.id = curr.id)
		{
			openList.erase(openList.begin() + i);
		}
	}
}

Node RTSGameManger::findLowestFScoringNode(Node dest)
{
	Node lowestFCost;
	lowestFCost.fCost = 0;

	for (Node node : openList)
	{
		if (lowestFCost.fCost > 0)
		{
			if (node.fCost < lowestFCost.fCost)
			{
				lowestFCost = node;
			}
		}
		else
		{
			lowestFCost = node;
		}
	}
	return lowestFCost;
}

float RTSGameManger::findDistanceH(Node current, Node dest)
{
	float x = abs(dest.cordinates.x - current.cordinates.x);
	float y = abs(dest.cordinates.y - current.cordinates.y);
	return sqrt(pow(x, 2.0f) + pow(y, 2.0f));
}

bool RTSGameManger::isNodeInList(Node curr,vector<Node> list)
{
	for(Node node : list)
	{
		if (node.id == curr.id)
		{
			return true;
		}
	}
	return false;
}

//checks path is vaild 
/*bool RTSGameManger::isDestination(float x, float z, Node dest)
//{
//	//if (x == dest.x && z == dest.z) {
//	//	return true;
//	//}
//	return false;
//}
//
//float RTSGameManger::calculateH(float x, float z, Node dest)
//{
//	float H = (sqrt((x - dest.x)*(x - dest.x)
//		+ (z - dest.z)*(z - dest.z)));
//	return H;
}*/

void RTSGameManger::pathFind(int modelId,XMFLOAT3 destination)
{
	/*need a better way to get a node/units cordinates*/
	unitLeader = units[modelId];
	unitLeader.Leader = true;
	units[modelId] = unitLeader;
	Node unitNode = gridMap[(int)unitLeader.cordinates.x][(int)unitLeader.cordinates.y];

	//destination = model::getInstancePos(GridSize-1);
	Node dest = gridMap[(int)destinationCor.x][(int)destinationCor.y];
	
	
	if (AStar(unitNode, dest))
	{
		pathFound = true;
	}
}

#pragma endregion

