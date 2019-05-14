#pragma once
#include "model.h"
class SpacShip : public model
{
public:
	SpacShip(HINSTANCE hInstance);
	~SpacShip();

	bool Init(ID3D11Device*, const WCHAR*, int NumberOfModles) override;
	void rotateToHeading();

private:
	bool initializeShipVertices(ID3D11Device* device);
	bool initializeShipIndexBuffer(ID3D11Device* device);
};

