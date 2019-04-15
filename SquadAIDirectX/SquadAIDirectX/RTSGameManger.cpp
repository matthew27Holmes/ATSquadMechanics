#include "RTSGameManger.h"



RTSGameManger::RTSGameManger(HINSTANCE hInstance) : model(hInstance)
{
	NumberOfModles = 4;
	GridHeight = 50;
	GridWidth = 50;
	GridSize = GridWidth * GridHeight;
	gridMap = new Node* [GridHeight];
	for (int i = 0; i < GridHeight; ++i)
		gridMap[i] = new Node[GridWidth];
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
			if (i == GridSize - 1)
			{
				postion.y = 2.0f;// far corner
				destinationTemp.x = i;
				destinationTemp.y = k;
			}
			model::addInstance(modelID, postion, scale, rotaion);
			Node nwNode;
			nwNode.postion = postion;
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
		nwUnit.postion = { posModX * 2, 2.0f, posModZ * 2 };;
		model::addInstance(modelID, nwUnit.postion, scale, rotaion);

		nwUnit.unitID = modelID;
		nwUnit.cordinates.x = nwUnit.postion.x;
		nwUnit.cordinates.y = nwUnit.postion.z;

		units[u] = nwUnit;

		if (posModX >= 2)
		{
			posModX = 0.0f;
			posModZ += 1.0f;
		}
	}

}

vector<Node> RTSGameManger::AStar(Node unitLeaderNode, Node dest)
{
	vector<Node> empty;
	openList.push_back(unitLeaderNode);
	
	//node with lowest h
	while (!openList.empty())
	{
		Node currentNode = findLowestFScoringNode(dest);
		if (currentNode.id == dest.id)
		{
			return closedList;
		}
		else
		{
			closedList.push_back(currentNode);
			openList.pop_back();
			addNeighbours(currentNode.cordinates.x, currentNode.cordinates.y);
		}
	}

	return empty;
}

/*float RTSGameManger::addGhistory(Node current)
{
	return 0.0f;
}*/

void RTSGameManger::addNeighbours(int x,int y)
{
	for (int i = 0; i <= 8; i++)
	{
		if (y > 1 && y < GridHeight
			&& x > 1 && x < GridWidth)
		{
			Node neighbour;

			switch (i)
			{
			case 0:
				neighbour = gridMap[--y][--x];
				neighbour.gCost = 1.4f;//need to do it based on distances not a flat rate // seperate function to look up how far this cordinate is from the player 
				break;
			case 1:
				neighbour = gridMap[y][--x];
				neighbour.gCost = 1.0f;
				break;
			case 2:
				neighbour = gridMap[++y][--x];
				neighbour.gCost = 1.4f;
				break;
			case 3:
				neighbour = gridMap[++y][x];
				neighbour.gCost = 1.0f;
				break;
			case 4:
				neighbour = gridMap[--y][x];
				neighbour.gCost = 1.0f;
				break;
			case 5:
				neighbour = gridMap[--y][++x];
				neighbour.gCost = 1.4f;
				break;
			case 6:
				neighbour = gridMap[y][++x];
				neighbour.gCost = 1.0f;
				break;
			case 7:
				neighbour = gridMap[++y][++x];
				neighbour.gCost = 1.4f;
				break;
			}


			if (!isNodeInList(neighbour, openList))
			{
				if (!isNodeInList(neighbour, closedList))
				{
					openList.push_back(neighbour);
				}
			}
		}
	}
}

Node RTSGameManger::findLowestFScoringNode(Node dest)
{
	Node lowestFCost;
	lowestFCost.fCost = 0;

	for (Node node : openList)
	{
		if (node.IsWalkable)
		{
			// calclualate eachs h value 
			openList.push_back(node);
			node.hCost = findDistanceH(node, dest); // find distance from this node to destination
			node.fCost = node.hCost + node.gCost;
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



#pragma region PathFinding
//checks path is vaild 
/*bool RTSGameManger::isValid(float x, float z)// this cane be done better
//{
//	 ////If our Node is an obstacle it is not valid
//		//int id = x + y * (X_MAX / X_STEP);
//		//if (world.obstacles.count(id) == 0) {
//		//	if (x < 0 || y < 0 || x >= (X_MAX / X_STEP) || y >= (Y_MAX / Y_STEP)) {
//		//		return false;
//		//	}
//		//	return true;
//		//}
//		return false;
//	
//}
*/
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

/*vector<Node> RTSGameManger::aStar(Unit player, Node dest)
{
	vector<Node> empty;
	//if (isValid(dest.x, dest.y) == false) {
	//	return empty;
	//	//Destination is invalid
	//}
	if (isDestination(player.postion.x, player.postion.z, dest)) {
		return empty;
		//You clicked on yourself
	}
	bool closedList[(X_MAX / X_STEP)][(Y_MAX / Y_STEP)];

	//Initialize whole map
	//Node allMap[50][25];
	array<array < Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> allMap;
	for (int x = 0; x < (X_MAX / X_STEP); x++) {
		for (int y = 0; y < (Y_MAX / Y_STEP); y++) {
			allMap[x][y].fCost = FLT_MAX;
			allMap[x][y].gCost = FLT_MAX;
			allMap[x][y].hCost = FLT_MAX;
			allMap[x][y].parentX = -1;
			allMap[x][y].parentZ = -1;
			allMap[x][y].x = x;
			allMap[x][y].z = y;

			closedList[x][y] = false;
		}
	}

	//Initialize our starting list
	int x = player.postion.x;
	int z = player.postion.z;
	allMap[x][z].fCost = 0.0;
	allMap[x][z].gCost = 0.0;
	allMap[x][z].hCost = 0.0;
	allMap[x][z].parentX = x;
	allMap[x][z].parentZ = z;

	vector<Node> openList;
	openList.emplace_back(allMap[x][z]);
	bool destinationFound = false;

	while (!openList.empty() && openList.size()<(X_MAX / X_STEP)*(Y_MAX / Y_STEP)) {
		Node node;
		do {
			//This do-while loop could be replaced with extracting the first
			//element from a set, but you'd have to make the openList a set.
			//To be completely honest, I don't remember the reason why I do
			//it with a vector, but for now it's still an option, although
			//not as good as a set performance wise.
			float temp = FLT_MAX;
			vector<Node>::iterator itNode;
			for (vector<Node>::iterator it = openList.begin();
				it != openList.end(); it = next(it)) {
				Node n = *it;
				if (n.fCost < temp) {
					temp = n.fCost;
					itNode = it;
				}
			}
			node = *itNode;
			openList.erase(itNode);
		} while (isValid(node.x, node.z) == false);

		x = node.x;
		z = node.z;
		closedList[x][z] = true;

		//For each neighbour starting from North-West to South-East
		for (int newX = -1; newX <= 1; newX++) {
			for (int newZ = -1; newZ <= 1; newZ++) {
				double gNew, hNew, fNew;
				if (isValid(x + newX, z + newZ)) {
					if (isDestination(x + newX, z + newZ, dest))
					{
						//Destination found - make path
						allMap[x + newX][z + newZ].parentX = x;
						allMap[x + newX][z + newZ].parentZ = z;
						destinationFound = true;
						return makePath(allMap, dest);
					}
					else if (closedList[x + newX][z + newZ] == false)
					{
						gNew = node.gCost + 1.0;
						hNew = calculateH(x + newX, z + newZ, dest);
						fNew = gNew + hNew;
						// Check if this path is better than the one already present
						if (allMap[x + newX][z + newZ].fCost == FLT_MAX ||
							allMap[x + newX][z + newZ].fCost > fNew)
						{
							// Update the details of this neighbour node
							allMap[x + newX][z + newZ].fCost = fNew;
							allMap[x + newX][z + newZ].gCost = gNew;
							allMap[x + newX][z + newZ].hCost = hNew;
							allMap[x + newX][z + newZ].parentX = x;
							allMap[x + newX][z + newZ].parentZ = z;
							openList.emplace_back(allMap[x + newX][z + newZ]);
						}
					}
				}
			}
		}
	}
	if (destinationFound == false) {
		// "Destination not found"
		return empty;
	}
}

vector<Node> RTSGameManger::makePath(array<array<Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> map, Node dest)
{
	try {
		//"Found a path"
		int x = dest.x;
		int z = dest.z;
		stack<Node> path;
		vector<Node> usablePath;

		while (!(map[x][z].parentX == x && map[x][z].parentZ == z)
			&& map[x][z].x != -1 && map[x][z].z != -1)
		{
			path.push(map[x][z]);
			int tempX = map[x][z].parentX;
			int tempZ = map[x][z].parentZ;
			x = tempX;
			z = tempZ;

		}
		path.push(map[x][z]);

		while (!path.empty()) {
			Node top = path.top();
			path.pop();
			usablePath.emplace_back(top);
		}
		return usablePath;
	}
	catch (const exception& e) {
		vector<Node> empty;
		return empty;
	}
}
*/

void RTSGameManger::pathFind(int modelId,XMFLOAT3 destination)
{
	Unit UnitLeader = units[modelId];
	//find units current postion
	Node unitNode = gridMap[(int)UnitLeader.cordinates.x][(int)UnitLeader.cordinates.y];

	destination = model::getInstancePos(GridSize);
	Node dest;
	dest.cordinates.x = destinationTemp.x;//destination.x;
	dest.cordinates.y = destinationTemp.y;//destination.z;

	for (Node node : AStar(unitNode, dest)) {
		//move model to each node postion
		updateInstancePos(UnitLeader.unitID, node.postion.x, UnitLeader.postion.y, node.postion.z);
	}
}

#pragma endregion

