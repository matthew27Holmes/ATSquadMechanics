#include "RTSGameManger.h"

RTSGameManger::RTSGameManger(HINSTANCE hInstance) : model(hInstance)
{
	NumberOfModles = 5;
	GridHeight = 100;
	GridWidth = 100;
	GridSize = GridWidth * GridHeight;
	gridMap = new Node* [GridHeight];
	for (int i = 0; i < GridHeight; ++i)
	{
		gridMap[i] = new Node[GridWidth];
	}
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
			nwUnit.pathFound = false;
			nwUnit.pathStep = 0;
			nwUnit.selected = true;//false;

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

void RTSGameManger::Render(float dt, ID3D11DeviceContext* deviceContext)
{
	model::RenderBuffers(deviceContext);
	return;
}

void RTSGameManger::Update(float dt, ID3D11Device *device)
{
	model::Update(dt);
	model::updateInstancesBuffer(device);

	for (int i = 0; i < units.size(); i++)
	{
		if (units[i].selected)
		{
			units[i] = updateUnitePos(units[i]);
		}
	}

	return;
}

Unit RTSGameManger::updateUnitePos(Unit unit)
{
	if (unit.pathFound)
	{
		XMFLOAT3 currentPos = getInstancePos(unit.unitID);
		XMFLOAT3 goalPos = XMFLOAT3(unit.path[unit.pathStep].position.x, unit.position.y, unit.path[unit.pathStep].position.z);

		moveTo(unit.unitID, goalPos);

		if (currentPos.x == unit.path[unit.pathStep].position.x && currentPos.z == unit.path[unit.pathStep].position.z)
		{
			unit.cordinates = unit.path[unit.pathStep].cordinates;
			

			if (unit.pathStep <= unit.path.size() - 2)
			{
				unit.pathStep++;
			}
			else
			{
				unit.pathStep = unit.path.size() - 1;
				unit.pathFound = false;
				unit.path.clear();
			}
		}
		//updateInstancePos(UnitLeader.unitID, node.postion.x, UnitLeader.postion.y, node.postion.z);
	}
	return unit;
}

#pragma region PathFinding

bool RTSGameManger::AStar(Node startNode, Node dest, int unitID)
{
	closedList.clear();
	openList.clear();
	if (!isDestination(startNode, dest))
	{
		openList.push_back(startNode);
		while (!openList.empty())
		{
			Node currentNode = findLowestFScoringNode();
			//gridMap[(int)currentNode.cordinates.x][(int)currentNode.cordinates.y] = currentNode;
			if (isDestination(currentNode, dest))
			{
				// add all parents form the closed list 
				units[unitID].path.clear();
				units[unitID].path = createPath(currentNode, startNode);
				units[unitID].pathStep = 0;
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
		parnetNode = gridMap[(int)parnetNode.parentCordinates.x][(int)parnetNode.parentCordinates.y];
		temp.push_back(parnetNode);
	}
	reverse(temp.begin(), temp.end());
	return temp;
}

void RTSGameManger::addNeighbours(int x, int y, Node dest,Node parent)
{
	if ( y < GridHeight -1
		 && x < GridWidth - 1)
	{
		Node neighbour;

		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (y + i > 0 && x + j > 0)
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

Node RTSGameManger::findLowestFScoringNode()
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

void RTSGameManger::pathFind(int LeaderID,int destination)//leader and dest
{

	Node unitNode = gridMap[(int)units[LeaderID].cordinates.x][(int)units[LeaderID].cordinates.y];

	// find destination in grid 
	Node dest = findNodeInMap(destination);//gridMap[(int)destinationCor.x][(int)destinationCor.y];// 
	units[LeaderID].dest = dest;

	if (!units[LeaderID].pathFound)//means you cant move again until you arrive at the dest
	{
		if (AStar(unitNode, dest, LeaderID))
		{
			units[LeaderID].pathFound = true;
			for (int i = 0; i < units.size(); i++)
			{
				if (i != LeaderID)
				{
					if (units[i].selected)
					{
						units[i].dest = floodFill(dest);
						unitNode = gridMap[(int)units[i].cordinates.x][(int)units[i].cordinates.y];

						if (!units[i].pathFound)
						{
							if (AStar(unitNode, units[i].dest, i))
							{
								units[i].pathFound = true;
							}
						}
					}
				}
			}
		}
	}
}

#pragma endregion

#pragma region unitControl

Node RTSGameManger::floodFill(Node orginNode)
{
	int row[] = { -1, -1, -1,  0, 0,  1, 1, 1 };
	int col[] = { -1,  0,  1, -1, 1, -1, 0, 1 };
	int neighboursMod = 1;
	int x = orginNode.cordinates.x, y = orginNode.cordinates.y;

	bool unitePlaced = false;

	while (!unitePlaced)
	{

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
					unitePlaced = true;

					return nwNode;
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
			if ((unit.dest.id == currNode.id))
			{
				return false;
			}
		}
		return true;
	}
	return false;
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

void RTSGameManger::toggleSelectUnite(int uniteId)
{
	for (int i = 0 ; i < units.size();i++)
	{
		if (units[i].unitID == uniteId)
		{
			if (units[i].selected)
			{
				units[i].selected = false;
			}
			else
			{
				units[i].selected = true;
			}
		}
	}
}
//check if there are any currently selected units
bool RTSGameManger::areUnitsSelected()
{
	for (Unit unit : units)
	{
		if (unit.selected)
		{
			return true;
		}
	}
	return false;
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

