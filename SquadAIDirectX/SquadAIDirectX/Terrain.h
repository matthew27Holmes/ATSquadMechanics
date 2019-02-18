#pragma once
#include "model.h"

class Terrain : public model
{
public:
	Terrain(HINSTANCE hInstance);
	~Terrain()override;

	bool Init(ID3D11Device*);
	void Update(float dt)override;

	void Render(float dt) override;
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	XMMATRIX getTerrainMatrix();

	int GetIndexCount();
private:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	bool InitializeBuffers(ID3D11Device*);
	bool LoadVerticesIndicesArray(VertexType* vertices, unsigned long* indices);

	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount, m_indexCount;
	XMMATRIX TerrainMatrix, m_fudge;

	float posX, posY, posZ, roll, pitch, yaw, scaleX, scaleY, scaleZ;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
};

