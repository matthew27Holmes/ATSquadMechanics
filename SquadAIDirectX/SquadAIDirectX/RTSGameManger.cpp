#include "RTSGameManger.h"

RTSGameManger::RTSGameManger(HINSTANCE hInstance) : model(hInstance)
{
	NumberOfModles = 10;
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
			unitLeader.cordinates = path[pathStep].cordinates;
			floodFill(path[pathStep], unitLeader);

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

#pragma region createGame
void RTSGameManger::createGrid()
{
	int modelID = 0;
	srand((unsigned)time(NULL));

	for (int k = 0; k < GridHeight; k++)
	{
		for (int i = 0; i < GridWidth; i++)
		{
			Node nwNode;
			nwNode.IsWalkable = true;

			XMFLOAT3 rotaion = { 0.0f, 0.0f, 0.0f };
			XMFLOAT3 scale = { 1.0f, 0.1f, 1.0f };
			XMFLOAT3 postion = { (float)i * 2, 1.0f, (float)k * 2 };
			// random chance to create an obsticle increase scale and set not walkable add to obsticle lsit

			//if i dont get picking in then i should set it so that dest cant be an obsticle 
			if (i == GridWidth - 1 && k == GridHeight - 1)
			{
				destinationCor.x = i;
				destinationCor.y = k;
			}

			int randNum = rand() % 100 + 1;//0-100
			if (randNum < 20)
			{
				//scale.y = 2.5f;
				//postion.y = 2.5f;
				rotaion.x = 22;
				nwNode.IsWalkable = false;
			}

			nwNode.position = postion;
			nwNode.id = modelID;
			nwNode.cordinates.x = i;
			nwNode.cordinates.y = k;
			gridMap[i][k] = nwNode;
			model::addInstance(modelID, postion, scale, rotaion);
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
	for (int u = 0; u < NumberOfModles;)
	{
		posModX += 1.0f;
		if (isNodeVaild(gridMap[(int)posModX * 2][(int)posModZ * 2]))//this isnt working
		{
			modelID++;

			XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
			XMFLOAT3 rotaion = { 0.0f, 0.0f, 0.0f };
			posModX += 1.0f;
			nwUnit.position = { posModX * 2, 2.0f, posModZ * 2 };;
			model::addInstance(modelID, nwUnit.position, scale, rotaion);

			nwUnit.unitID = modelID;
			nwUnit.cordinates.x = nwUnit.position.x;
			nwUnit.cordinates.y = nwUnit.position.z;

			units[u] = nwUnit;
			u++;
		}

		if (posModX >= 2)
		{
			posModX = 0.0f;
			posModZ += 1.0f;
		}
	}
}

#pragma endregion

#pragma region unitControl

void RTSGameManger::floodFill(Node currStep, Unit Leader)
{
	int row[] = { -1, -1, -1,  0, 0,  1, 1, 1 };
	int col[] = { -1,  0,  1, -1, 1, -1, 0, 1 };
	int neighboursMod = 1;
	int x = Leader.cordinates.x, y = Leader.cordinates.y;

	vector<Unit> unPlacedUnits;

	for (Unit unit : units)//shoud be selected units
	{
		if (unit.unitID != Leader.unitID)
		{
			unPlacedUnits.push_back(unit);
		}
	}
	units.clear();
	units.push_back(Leader);


	while (!unPlacedUnits.empty())
	{
		bool unitePlaced = false;
		Unit unite = unPlacedUnits.back();
		
		for (int k = 0; k < 8; k++)
		{
			int neighboursX = x + (row[k] * neighboursMod);
			int neighboursY = y + (col[k] * neighboursMod);

			if (neighboursY > 1 && neighboursY <= GridHeight - 1
				&& neighboursX > 1 && neighboursX <= GridWidth - 1)
			{
				Node nwNode = gridMap[neighboursX][neighboursY];

				if (isNodeVaild(nwNode))
				{
					unPlacedUnits.pop_back();
					unitePlaced = true;
					neighboursMod = 1;
					k = 8;

					unite.cordinates = nwNode.cordinates;
					//replace in unites array
					units.push_back(unite);
					updateInstancePos(unite.unitID, nwNode.position.x, unite.position.y, nwNode.position.z);
					unite.position = nwNode.position;
				}
			}
		}
		if (!unitePlaced)
		{
			neighboursMod++;
		}
	}
}

bool RTSGameManger::isNodeVaild(Node currNode)
{
	if (currNode.IsWalkable)//&& other unit isnt in it already 
	{
		for (Unit unit : units)//shoud be selected units
		{
			if ((unit.cordinates.x == currNode.cordinates.x) && (unit.cordinates.y == currNode.cordinates.y))
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

#pragma endregion

#pragma region PathFinding

bool RTSGameManger::AStar(Node unitLeaderNode, Node dest)
{
	closedList.clear();
	openList.clear();
	path.clear();
	if (!isDestination(unitLeaderNode, dest))
	{
		openList.push_back(unitLeaderNode);
		while (!openList.empty())
		{
			Node currentNode = findLowestFScoringNode(dest);
			//gridMap[(int)currentNode.cordinates.x][(int)currentNode.cordinates.y] = currentNode;
			if (isDestination(currentNode, dest))
			{
				// add all parents form the closed list 
				path = createPath(currentNode, unitLeaderNode);
				return true;
			}
			else
			{
				closedList.push_back(currentNode);
				openList = removeNodeFromList(currentNode, openList);
				addNeighbours(currentNode.cordinates.x, currentNode.cordinates.y, dest, currentNode);
			}
		}
	}
	return false;
}

vector<Node> RTSGameManger::createPath(Node curr, Node startNode)
{
	// backtrack through all parent nodes until you reach the start
	vector<Node> temp;
	Node parnetNode = gridMap[(int)curr.cordinates.x][(int)curr.cordinates.y];
	temp.push_back(parnetNode);

	while (parnetNode.id != startNode.id)
	{
		if (parnetNode.id == 104)
		{
			int i = 0;
		}
		parnetNode = gridMap[(int)parnetNode.parentCordinates.x][(int)parnetNode.parentCordinates.y];
		temp.push_back(parnetNode);
	}
	reverse(temp.begin(), temp.end());
	pathStep = 0;
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
				neighbour = gridMap[x + j][y + i];
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
							gridMap[x + j][y + i] = neighbour;
							openList.push_back(neighbour);
						}
					}
				}
			}
		}
	}
}

vector<Node>  RTSGameManger::removeNodeFromList(Node curr, vector<Node>List)
{
	for (int i = 0; i < List.size(); i++)
	{
		Node temp = List[i];
		if (temp.id = curr.id)
		{
			List.erase(List.begin() + i);
		}
	}
	return List;
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

void RTSGameManger::pathFind(int uniteID,int destination)
{
	/*minus grid size from unite ID */
	unitLeader = units[uniteID];
	unitLeader.Leader = true;
	units[uniteID] = unitLeader;
	Node unitNode = gridMap[(int)unitLeader.cordinates.x][(int)unitLeader.cordinates.y];

	// find destination in grid 
	Node dest = findNodeInMap(destination);//gridMap[(int)destinationCor.x][(int)destinationCor.y];// 
 	if (!pathFound)//means you cant move again until you arrive at the dest
	{
		if (AStar(unitNode, dest))
		{
			pathFound = true;
		}
	}
}

#pragma endregion

#pragma region pathFindingHelperFunctions

Node RTSGameManger::findNodeInMap(int nodeID)
{
	for (int k = 0; k < GridHeight; k++)
	{
		for (int i = 0; i < GridWidth; i++)
		{
			Node foundNode = gridMap[i][k];
			if (foundNode.id == nodeID)
			{
				return foundNode;
			}
		}

	}
}

bool RTSGameManger::isNodeInList(Node curr, vector<Node> list)
{
	for (Node node : list)
	{
		if (node.id == curr.id)
		{
			return true;
		}
	}
	return false;
}

bool RTSGameManger::isDestination(Node currNode, Node dest)
{
	if (currNode.id == dest.id)
	{
		return true;
	}
	return false;
}

//check route will be achiveable before you leave
#pragma endregion

