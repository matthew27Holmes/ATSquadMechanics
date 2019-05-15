#include "RTSGameManger.h"

RTSGameManger::RTSGameManger(HINSTANCE hInstance)
{
	NumberOfModles = 10;
	GridHeight = 50;
	GridWidth = 50;
	GridSize = GridWidth * GridHeight;

	wallNum = GridWidth * 2 + GridHeight * 2;
	ships = new SpacShip(hInstance);
	Tiles = new Tile(hInstance);
	creates = new model(hInstance);
	barrels = new model(hInstance);
	boxs = new model(hInstance);
	walls = new model(hInstance);

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

	if (!Tiles->Init(device, L"../SquadAIDirectX/Resource/Stone.PNG", "../SquadAIDirectX/Resource/cube.txt", GridSize+1))
	{
		OutputDebugString("Could not initialize the tiles object.");
		return false;
	}	
	
	if (!creates->Init(device, L"../SquadAIDirectX/Resource/Stone.PNG", "../SquadAIDirectX/Resource/cube.txt", createNum))
	{
		OutputDebugString("Could not initialize the tiles object.");
		return false;
	}	
	/*if (!barrels->Init(device, L"../SquadAIDirectX/Resource/Stone.PNG", "../SquadAIDirectX/Resource/Barrell.txt", bralleNum))
	{
		OutputDebugString("Could not initialize the tiles object.");
		return false;
	}	*/
	if (!boxs->Init(device, L"../SquadAIDirectX/Resource/Stone.PNG", "../SquadAIDirectX/Resource/cube.txt", boxNum))
	{
		OutputDebugString("Could not initialize the tiles object.");
		return false;
	}	
	if (!walls ->Init(device, L"../SquadAIDirectX/Resource/Stone.PNG", "../SquadAIDirectX/Resource/cube.txt",wallNum))
	{
		OutputDebugString("Could not initialize the tiles object.");
		return false;
	}	
	
	if (!ships->Init(device, L"../SquadAIDirectX/Resource/Stone.PNG", "../SquadAIDirectX/Resource/drone.txt",NumberOfModles))
	{
		OutputDebugString("Could not initialize the model object.");
		return false;
	}	

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
			int texture = 0;

			int randNum = rand() % 100 + 1;//0-100
			if ((i==0||i==GridWidth-1)||(k==0||k==GridHeight-1))
			{
				XMFLOAT3 nwscale = { 1.0f, 5.5f, 1.0f };
				XMFLOAT3 nwpostion = { postion.x, 3.5f, postion.z };
				walls->addInstance(currWallNum, nwpostion, nwscale, rotaion, 4);
				currWallNum++;
				nwNode.IsWalkable = false;
			}
			else if (randNum < 4)
			{
				int ran = rand() % 100 + 1;//0-100
				if (ran > 50)
				{
					if (currCreateNum < createNum)
					{
						XMFLOAT3 nwscale = { 1.0f, (float)randNum, 1.0f };
						XMFLOAT3 nwpostion = { postion.x, (((float)randNum)/ 0.7f), postion.z };
						creates->addInstance(currCreateNum, nwpostion, nwscale, rotaion, 1);
						currCreateNum++;
						nwNode.IsWalkable = false;

					}	
				}
				else 
				{
					if (currBoxNum < boxNum)
					{
						XMFLOAT3 nwscale = { 1.0f, 1.0f, 1.0f };
						XMFLOAT3 nwpostion = { postion.x, 2.0f, postion.z };
						boxs->addInstance(currBoxNum, nwpostion, nwscale, rotaion, 6);
						currBoxNum++;
						nwNode.IsWalkable = false;
					}
				}
			}
			

			nwNode.position = postion;
			nwNode.id = modelID;
			nwNode.cordinates.x = i;
			nwNode.cordinates.y = k;
			gridMap[i][k] = nwNode;
			Tiles->addInstance(modelID, postion, scale, rotaion, texture);// add texture to add instances
			modelID++;
		}
	}

	//XMFLOAT3 rotaion = { 0.0f, 0.0f, 0.0f };
	//XMFLOAT3 scale = { 100.0f, 1.0f, 100.0f };
	//XMFLOAT3 postion = { (float)GridWidth-1, 10.0f,(float)GridHeight-1 };
	//
	//Tiles->addInstance((GridHeight-1)+ GridWidth, postion, scale, rotaion, 4);// add texture to add instances

}

void RTSGameManger::createUnits()
{
	// create blue team 
	float posModX = 0.0f, posModZ = 0.0f;
	//create units 
	Unit nwUnit;
	units.assign(NumberOfModles, nwUnit);

	int u = 0;
	for (; u < NumberOfModles/2;)
	{
		if (isNodeVaild(gridMap[(int)posModX][(int)posModZ]))//this isnt working
		{
			//modelID++;
			Node currNode = gridMap[(int)posModX][(int)posModZ];

			XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
			XMFLOAT3 rotaion = { 0.0f, 0.0f, 0.0f };
			nwUnit.position = {currNode.position.x, 2.0f, currNode.position.z};
			int texture = 2;
			ships->addInstance(u, nwUnit.position, scale, rotaion, texture);

			nwUnit.unitID = u;
			nwUnit.cordinates.x = currNode.cordinates.x;//nwUnit.position.x;
			nwUnit.cordinates.y = currNode.cordinates.y;//nwUnit.position.z;
			nwUnit.pathFound = false;
			nwUnit.pathStep = 0;
			nwUnit.selected = false;
			nwUnit.alive = true;
			nwUnit.team = "Blue";

			units[u] = nwUnit;
			u++;
		}
		posModX ++;
		if (posModX >= GridWidth-1)
		{
			posModX = 0.0f;
			posModZ += 1.0f;
		}
	}
	//create red team 
	posModX = GridWidth-1, posModZ = GridHeight-1;
	for (; u < NumberOfModles;)
	{
		if (isNodeVaild(gridMap[(int)posModX][(int)posModZ]))//this isnt working
		{
			//modelID++;
			Node currNode = gridMap[(int)posModX][(int)posModZ];

			
			XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
			XMFLOAT3 rotaion = { 0.0f, 0.0f, 0.0f };// roatate them 
			nwUnit.position = { currNode.position.x, 2.0f, currNode.position.z };
			int texture = 3;
			ships->addInstance(u, nwUnit.position, scale, rotaion, texture);

			nwUnit.unitID = u;
			nwUnit.cordinates.x = currNode.cordinates.x;
			nwUnit.cordinates.y = currNode.cordinates.y;
			nwUnit.pathFound = false;
			nwUnit.pathStep = 0;
			nwUnit.selected = false;
			nwUnit.alive = true;
			nwUnit.team = "Red";

			units[u] = nwUnit;
			u++;
		}
		posModX--;
		if (posModX <= 0)
		{
			posModX = GridWidth - 1;
			posModZ -=1.0F;
		}
	}
}

#pragma endregion

void RTSGameManger::Render(float dt, ID3D11DeviceContext* deviceContext, textureShader* texture)
{
	Tiles->RenderBuffers(deviceContext, texture);
	walls->RenderBuffers(deviceContext, texture);
	creates->RenderBuffers(deviceContext, texture);
	boxs->RenderBuffers(deviceContext, texture);
	barrels->RenderBuffers(deviceContext, texture);
	ships->RenderBuffers(deviceContext, texture);
	return;
}

void RTSGameManger::Update(float dt, ID3D11Device *device)
{
	Tiles->Update(dt);
	Tiles->updateInstancesBuffer(device);

	walls->Update(dt);
	walls->updateInstancesBuffer(device);
	creates->Update(dt);
	creates->updateInstancesBuffer(device);
	boxs->Update(dt);
	boxs->updateInstancesBuffer(device);
	barrels->Update(dt);
	barrels->updateInstancesBuffer(device);

	ships->Update(dt);
	ships->updateInstancesBuffer(device);

	for (int i = 0; i < units.size(); i++)
	{
		if (units[i].selected && units[i].alive)
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
		XMFLOAT3 currentPos = ships->getInstancePos(unit.unitID);
		XMFLOAT3 goalPos = XMFLOAT3(unit.path[unit.pathStep].position.x, unit.position.y, unit.path[unit.pathStep].position.z);

		ships->moveTo(unit.unitID, goalPos);

		if (currentPos.x == unit.path[unit.pathStep].position.x && currentPos.z == unit.path[unit.pathStep].position.z)
		{
			unit.cordinates = unit.path[unit.pathStep].cordinates;
			unitsAttacking(unit);

			if (unit.pathStep <= unit.path.size() - 2)
			{
				unit.pathStep++;
				ships->rotateToHeading(unit.unitID, unit.path[unit.pathStep].position);
			}
			else
			{
				unit.pathStep = unit.path.size() - 1;
				unit.pathFound = false;
				unit.path.clear();
			}
		}
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

void RTSGameManger::addNeighbours(int x, int y, Node dest, Node parent)
{
	Node neighbour;

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (y + i <= GridHeight - 1
				&& x + j <= GridWidth - 1)
			{
				if (y + i >= 0 && x + j >= 0)
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

vector<Node> RTSGameManger::removeNodeFromList(Node curr, vector<Node>List)
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

void RTSGameManger::findPath(int LeaderID,int destination)//leader and dest
{
	Node unitNode = gridMap[(int)units[LeaderID].cordinates.x][(int)units[LeaderID].cordinates.y];

	// find destination in grid 
	Node dest = findNodeInMap(destination);//gridMap[(int)destinationCor.x][(int)destinationCor.y];// 
	units[LeaderID].dest = dest;

	if (!units[LeaderID].pathFound&&dest.IsWalkable)//means you cant move again until you arrive at the dest
	{
		if (AStar(unitNode, dest, LeaderID))
		{
			units[LeaderID].pathFound = true;
			for (int i = 0; i < units.size(); i++)
			{
				if (i != LeaderID)
				{
					if (units[i].selected && units[i].alive)
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
	int neighboursModX = 1;
	int neighboursModY = 1;
	int x = orginNode.cordinates.x, y = orginNode.cordinates.y;

	bool unitePlaced = false;

	while (!unitePlaced)
	{

		for (int k = 0; k < 8; k++)
		{
			int neighboursX = x + (row[k] * neighboursModX);
			int neighboursY = y + (col[k] * neighboursModY);

			if (neighboursY <= GridHeight - 1
				&& neighboursX <= GridWidth - 1)
			{
				if (neighboursY >= 0 && neighboursX >= 0)
				{
					Node nwNode = gridMap[neighboursX][neighboursY];

					if (isNodeVaild(nwNode))
					{
						unitePlaced = true;

						return nwNode;
					}
				}
			}
		}
		if (!unitePlaced)
		{
			neighboursModX++;
			if (neighboursModX > 3)
			{
				neighboursModY++;
				neighboursModX = 0;
			}
		}
	}
}

bool RTSGameManger::isNodeVaild(Node currNode)
{
	if (currNode.IsWalkable)//&& other unit isnt in it already 
	{
		for (Unit unit : units)//shoud be selected units
		{
			if (unit.cordinates.x == currNode.cordinates.x && unit.cordinates.y == currNode.cordinates.y)// and they are on the same team
			{
				return false;
			}
			if ((unit.dest.id == currNode.id))
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

void RTSGameManger::selectUnite(int uniteId)
{
	for (int i = 0; i < units.size(); i++)
	{
		if (units[i].unitID == uniteId)
		{
			if (units[i].alive)
			{
				units[i].selected = true;
			}
			return;
		}
	}
}
//check if there are any currently selected units
void RTSGameManger::deSelecteAll()
{
	for (int i = 0; i < units.size(); i++)
	{
		if (units[i].selected)
		{
			units[i].selected = false;
		}
	}
}

int RTSGameManger::getUniteByUnitID(int UniteID)
{
	int returnID = -1;// so you can tell if its found something or not
	for (int i = 0; i < units.size(); i++)
	{
		if (units[i].unitID == UniteID)
		{
			return i;
		}
	}
	return returnID;
}
void RTSGameManger::unitsAttacking(Unit currUnit)
{
	//run thorugh all selected units 
	for (int i = 0; i < NumberOfModles; i++)
	{
		Unit unit = units[i];
		if (unit.alive)
		{
			if (unit.team != currUnit.team)
			{
				if (unit.cordinates.x == currUnit.cordinates.x && unit.cordinates.y == currUnit.cordinates.y)
				{
					killUnit(i);
				}
			}
		}
	}
	
	// if any sahre the same space as another teams unit
	// kill other teams units 
}
void RTSGameManger::killUnit(int uniteID)
{
	Unit unit = units[uniteID];
	unit.selected = false;
	unit.alive = false;
	unit.position.y = -1.0f;;
	ships->updateInstancePos(unit.unitID, unit.cordinates.x, -1.0f, unit.cordinates.y);
}
int RTSGameManger::getIndexCount()
{
	return ships->getIndexCount() + Tiles->getIndexCount();
}
int RTSGameManger::getVertexCount()
{
	return 	ships->getVertexCount() + Tiles->getVertexCount();
}
int RTSGameManger::getInstanceCount()
{
	return 	ships->getInstanceCount() + Tiles->getInstanceCount();
}
int RTSGameManger::checkTileCollison(XMVECTOR rayDirc, XMVECTOR rayOrgin)
{
	return Tiles->checkCollison(rayDirc,rayOrgin);
}
int RTSGameManger::checkShipCollison(XMVECTOR rayDirc, XMVECTOR rayOrgin)
{
	return ships->checkCollison(rayDirc, rayOrgin);
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

