#include "SpacShip.h"



SpacShip::SpacShip(HINSTANCE hInstance) : model(hInstance)
{
}


SpacShip::~SpacShip()
{
}

bool SpacShip::Init(ID3D11Device *device, const WCHAR* textureFilename, int NumberOfModles)
{
	if (!initializeShipVertices(device))
	{
		OutputDebugString("Failed to create vertex buffer");
		return false;
	}

	if (!initializeShipIndexBuffer(device))
	{
		OutputDebugString("Failed to create index buffer");
		return false;
	}

	model::initializeInstance(NumberOfModles);
	return true;
}

void SpacShip::rotateToHeading()
{
	// find direction of movement 
	// ratotate to match

}

bool SpacShip::initializeShipVertices(ID3D11Device * device)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	HRESULT result;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 24;
	VertexType v;

	// Create the vertex array.
	vertices.assign(m_vertexCount, v);

	//Load the vertex array with data.

	// fuselage
	vertices[0].position = XMFLOAT3(0.5f, 0.0f, 0.0f);//BFL
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 0.5f, -0.5f);//BFL
	vertices[1].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[2].position = XMFLOAT3(0.0f, 0.0f, 1.5f);//BFL
	vertices[2].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[3].position = XMFLOAT3(-0.5f, 0.0f, 0.0f);//BFL
	vertices[3].texture = XMFLOAT2(1.0f, 1.0f);

	// left gun

	vertices[4].position = XMFLOAT3(0.7f, -0.2f, -0.5f);//BFL
	vertices[4].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[5].position = XMFLOAT3(0.7f, -0.2f, 1.2f);//BFL
	vertices[5].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[6].position = XMFLOAT3(0.0f, 0.2f, 0.3f);//BFL
	vertices[6].texture = XMFLOAT2(0.0f, 0.0f);

	// right gun

	vertices[7].position = XMFLOAT3(-0.7f, -0.2f, -0.5f);//BFL
	vertices[7].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[8].position = XMFLOAT3(-0.7f, -0.2f, 1.2f);//BFL
	vertices[8].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[9].position = XMFLOAT3(-0.0f, 0.2f, 0.3f);//BFL
	vertices[9].texture = XMFLOAT2(0.0f, 0.0f);


	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create vertex buffer");
		return false;
	}

	return true;
}

bool SpacShip::initializeShipIndexBuffer(ID3D11Device * device)
{

	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	HRESULT result;
	m_indexCount = 36;

	int i;
	// Create the vertex array.
	indices.assign(m_indexCount, i);

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 2; indices[4] = 1; indices[5] = 3;

	indices[6] = 3; indices[7] = 1; indices[8] = 0;
	indices[9] = 0; indices[10] = 2; indices[11] = 3;

	indices[12] = 4; indices[13] = 5; indices[14] = 6;
	indices[15] = 7; indices[16] = 8; indices[17] = 9;

	// Set the number of indices in the index array.

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create index buffer");
		return false;
	}
	return true;
}
