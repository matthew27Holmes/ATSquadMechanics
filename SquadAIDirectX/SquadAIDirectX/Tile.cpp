#include "Tile.h"

Tile::Tile(HINSTANCE hInstance) : model(hInstance)
{
}

Tile::~Tile()
{
}

bool Tile::Init(ID3D11Device *device, const WCHAR* textureFilename, int NumberOfModles)
{
	return model::Init(device, textureFilename, NumberOfModles);
}
