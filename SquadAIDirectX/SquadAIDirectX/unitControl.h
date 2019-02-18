#pragma once
#include "model.h"

//#include <Vector>
//using namespace std;

class unitControl : public model
{
public:
	unitControl(HINSTANCE hInstance);
	~unitControl() override;
	
	bool Init();

	void Render(float dt) override;
	void Update(float dt) override;

	void createGrid();// possible should be in this class 
	void createUnits();

	void setGoal(XMFLOAT3 goal);

	int findNextStepInPath();
	int findSquaresF(XMFLOAT3 SquareToCheck);
	void createOpenList();
	void moveUnits();

private: 

	//vector<XMFLOAT3> openList;
	//vector<XMFLOAT3> closedList;
	XMFLOAT3 currentPoint;
	XMFLOAT3 goalPos;

	int GridHeight, GridWidth, GridSize, NumberOfModles;
};

