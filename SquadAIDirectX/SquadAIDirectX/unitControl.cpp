#include "unitControl.h"



unitControl::unitControl(HINSTANCE hInstance) : model(hInstance)
{
	NumberOfModles = 4;
	GridHeight = 10;
	GridWidth = 10;
	GridSize = GridWidth * GridHeight;
}


unitControl::~unitControl()
{
}

bool unitControl::Init()
{
	//createGrid();
	//createUnits();
	return true;
}

void unitControl::Render(float dt)
{
}

void unitControl::Update(float dt)
{
}

void unitControl::createGrid()
{
	float posModX = 0.0f, posModZ = 0.0f;

	//create grid objects
	for (int i = 0; i < GridSize; i++)
	{
		
		posModX += 1.0f;
		model::updateInstancePos(i, posModX * 2, 1.0f, posModZ * 2);
		model::updateInstanceIsUnit(i,false);
		model::updateInstanceIsWalkable(i, true);


		if (posModX >= GridWidth)
		{
			posModX = 0.0f;
			posModZ += 1.0f;
		}
	}
}

void unitControl::createUnits()
{
	float posModX = 0.0f, posModZ = 0.0f;
	//create units 
	int modelIdentifier = GridSize - 1;// -1 zero index
	for (int i = 0; i < NumberOfModles; i++)
	{
		modelIdentifier++;

		posModX += 1.0f;
		model::updateInstancePos(modelIdentifier, posModX * 2, 1.0f, posModZ * 2);
		model::updateInstanceIsUnit(modelIdentifier, true);
		model::updateInstanceIsWalkable(modelIdentifier, false);


		if (posModX >= 2)
		{
			posModX = 0.0f;
			posModZ += 1.0f;
		}

	}
}

void unitControl::setGoal(XMFLOAT3 goal)
{
	goalPos = goal;
}

void unitControl::moveUnits()
{

}

int unitControl::findNextStepInPath()
{
	int currentF = 0;
	int lastF = 0;
	int LowestFIndcice = 0;

	/*for (int i; i < openList.size; i++)
	{
		currentF = findSquaresF(openList[i]);
		if (currentF < lastF)
		{
			lastF = currentF;
			LowestFIndcice = i;
		}
	}*/
	return LowestFIndcice;
}

int unitControl::findSquaresF(XMFLOAT3 SquareToCheck)
{
	int H = 10; // move cost
	float W = 0; // dostance to goal
	int F = 0; // finale weight

	//is moving diagnol
	if ((SquareToCheck.x == currentPoint.x + 1) && (SquareToCheck.y == currentPoint.y + 1))
	{
		H = 14;
	}
	
	// find distance from Sqaure being calculated to goal  
	XMVECTOR sqaureVector = XMLoadFloat3(&SquareToCheck);
	XMVECTOR goalVector = XMLoadFloat3(&goalPos);
	XMVECTOR vectorSub = XMVectorSubtract(sqaureVector, goalVector);
	XMVECTOR length = XMVector3Length(vectorSub);

	XMStoreFloat(&W, length);

	F = (int)W + H;
	return F;
}
