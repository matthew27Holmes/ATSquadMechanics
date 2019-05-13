#pragma once
#include "model.h"
class Tile :
	public model
{
public:
	Tile(HINSTANCE hInstance);
	~Tile();
	bool Init(ID3D11Device*, const WCHAR*, int NumberOfModles) override;
};

