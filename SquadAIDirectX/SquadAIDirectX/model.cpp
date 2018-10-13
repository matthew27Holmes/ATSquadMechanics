#include "model.h"

model::model(HINSTANCE hInstance) : DXApp(hInstance)
{
	m_vertexBuffer = 0;
	m_instanceCount = 0;
	m_pConstantBuffer = 0;
	//m_indexBuffer = 0;

	m_posX = 0.0f, m_posY = 0.0f, m_posZ = 0.0f;
	m_scaleX = 0.0f, m_scaleY = 0.0f, m_scaleZ = 0.0f;
	m_roll = 0.0f, m_yaw = 0.0f, m_pitch = 0.0f;
	modelMatrix = XMMatrixIdentity();
	m_fudge = XMMatrixIdentity();
}


model::~model()
{
	Memory::SafeRelease(m_instanceBuffer);
	Memory::SafeRelease(m_vertexBuffer);
}

bool model::Init(ID3D11Device* device)
{
	
	// Initialize the vertex and index buffers.
	if (!InitializeBuffers(device))
	{
		return false;
	}

	return true;

}

void model::Render(float dt)
{
	//scale = XMMatrixScaling(m_scaleX, m_scaleY, m_scaleY);
	return;
}

void model::Update(float dt)
{
	scaleMatrix = XMMatrixScaling(m_scaleX, m_scaleY, m_scaleZ);
	rotaionMatrix = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll);
	trasformMatrix = XMMatrixTranslation(m_posX, m_posY, m_posZ);

	modelMatrix = m_fudge * scaleMatrix *rotaionMatrix *trasformMatrix;
	return;
}


void model::setScale(float scalex, float sacley, float saclez)
{
	m_scaleX= scalex;
	m_scaleY= sacley;
	m_scaleZ= saclez;
}

void model::setRotaion(float pitch, float yaw, float roll)
{

	m_roll = roll;
	m_yaw = yaw;
	m_pitch = pitch;
}

int model::getInstanceCount()
{
	return m_instanceCount;
}


int model::getVertexCount()
{
	return m_vertexCount;
}

bool model::InitializeBuffers(ID3D11Device* device)
{

	CD3D11_BUFFER_DESC cbDesc(
		sizeof(ConstantBufferStruct),
		D3D11_BIND_CONSTANT_BUFFER
	);
	HRESULT hr = device->CreateBuffer(
		&cbDesc,
		nullptr,
		&m_pConstantBuffer
	);

	CreateCube(device);

	return true;
}

bool model::CreateCube(ID3D11Device* device)
{
	if (!createCubeVertices(device))
	{
		OutputDebugString("Failed to create vertex buffer");
		return false;
	}

	/*unsigned long*indices;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	HRESULT result;
	m_indexCount = 36;
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		OutputDebugString("Failed to create indecies");
	}

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	indices[6] = 4; indices[7] = 5; indices[8] = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;

	indices[12] = 3; indices[13] = 2; indices[14] = 5;
	indices[15] = 3; indices[16] = 5; indices[17] = 4;

	indices[18] = 2; indices[19] = 1; indices[20] = 6;
	indices[21] = 2; indices[22] = 6; indices[23] = 5;


	indices[24] = 1; indices[25] = 7; indices[26] = 6;
	indices[27] = 1; indices[28] = 0; indices[29] = 7;

	indices[30] = 0; indices[31] = 3; indices[32] = 4;
	indices[33] = 0; indices[34] = 4; indices[35] = 7;

	// Set the number of indices in the index array.
	//m_indexCount = sizeof(indices);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create index buffer");
	}

	delete[] indices;
	indices = 0;*/

	m_instanceCount = 4;

	if (!createInstances(device))
	{
		OutputDebugString("Failed to create instance buffer");
		return false;
	}

	return true;
}

/*
VertexType cubeVertices[] =
	{
	{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f,1.0f) }, // +Y (top face)
	{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f, 0.0f,1.0f) },
	{ XMFLOAT3(0.5f, 0.5f,  0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f,1.0f) },
	{ XMFLOAT3(-0.5f, 0.5f,  0.5f), XMFLOAT4(0.0f, 1.0f, 1.0f,1.0f) },

	{ XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f,1.0f) }, // -Y (bottom face)
	{ XMFLOAT3(0.5f, -0.5f,  0.5f), XMFLOAT4(1.0f, 0.0f, 1.0f,1.0f) },
	{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f,1.0f) },
	{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f,1.0f) },
	};

	unsigned short cubeIndices[] =
	{
	0, 1, 2,
	0, 2, 3,

	4, 5, 6,
	4, 6, 7,

	3, 2, 5,
	3, 5, 4,

	2, 1, 6,
	2, 6, 5,

	1, 7, 6,
	1, 0, 7,

	0, 3, 4,
	0, 4, 7
	};
}
*/

bool model::createCubeVertices(ID3D11Device* device)
{
	VertexType* vertices;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	HRESULT result;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 8;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		OutputDebugString("Failed to create vertices");
	}

	//Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(1.0f, 1.0f, -1.0f);
	vertices[1].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, 1.0f, 1.0f);
	vertices[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[3].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	vertices[3].color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);

	vertices[4].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	vertices[4].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	vertices[5].position = XMFLOAT3(1.0f, -1.0f, 1.0f);
	vertices[5].color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);

	vertices[6].position = XMFLOAT3(1.0f, -1.0f, -1.0f);
	vertices[6].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[7].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	vertices[7].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create vertex buffer");
		return false;
	}
	delete[] vertices;
	vertices = 0;

	return true;
}

bool model::createInstances(ID3D11Device* device)
{
	InstanceType* instances;
	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;

	HRESULT result;

	instances = new InstanceType[m_instanceCount];
	if (!instances)
	{
		return false;
	}

	// Load the instance array with data.
	instances[0].position = XMFLOAT3(-1.5f, -1.5f, 1.0f);
	instances[0].rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	instances[0].Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	instances[1].position = XMFLOAT3(-1.5f, 1.5f, 1.0f);
	instances[1].rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	instances[1].Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	instances[2].position = XMFLOAT3(1.5f, -1.5f, 1.0f);
	instances[2].rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	instances[2].Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	instances[3].position = XMFLOAT3(1.5f, 1.5f, 1.0f);
	instances[3].rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	instances[3].Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the instance buffer.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] instances;
	instances = 0;

	return true;
}


void model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	// Set the buffer strides.
	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(InstanceType);

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;

	/*deviceContext->UpdateSubresource(
		m_pConstantBuffer,
		0,
		nullptr,
		&m_constantBufferData,
		0,
		0
	);*/

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	/* Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);*/

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);//

	/*deviceContext->VSSetConstantBuffers(
		0,
		1,
		&m_pConstantBuffer
	);*/

	return;
}

void model::GetModelMatrix(XMMATRIX& nwModelMatrix)
{
	nwModelMatrix = modelMatrix;
	return;
}

void model::setPos(float x, float y, float z)
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;
}
