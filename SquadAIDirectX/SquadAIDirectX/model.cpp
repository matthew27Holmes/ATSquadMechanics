#include "model.h"

model::model(HINSTANCE hInstance) : DXApp(hInstance)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_instanceBuffer = 0;

	m_vertexCount = 8;
	boundingBox = new Collider();
	m_Texture = 0;
}

model::~model()
{
	Memory::SafeRelease(m_instanceBuffer);
	Memory::SafeRelease(m_vertexBuffer);
	Memory::SafeRelease(m_indexBuffer);
	Memory::SafeDelete(boundingBox);
	Memory::SafeDelete(m_Texture);
	
}

bool model::Init(ID3D11Device* device, WCHAR* textureFilename)
{
	if (!initializeCubeVertices(device))
	{
		OutputDebugString("Failed to create vertex buffer");
		return false;
	}

	if (!initializeIndexBuffer(device))
	{
		OutputDebugString("Failed to create index buffer");
		return false;
	}

	// Load the texture for this model.
	if (!LoadTexture(device, textureFilename))
	{
		return false;
	}

	return true;
}

#pragma region initialize

bool model::initializeCubeVertices(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	HRESULT result;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 8;
	VertexType v;

	// Create the vertex array.
	vertices.assign(m_vertexCount, v);

	//Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	//vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(1.0f, 1.0f, -1.0f);
	//vertices[1].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	vertices[1].texture = XMFLOAT2(0.5f, 0.0f);

	vertices[2].position = XMFLOAT3(1.0f, 1.0f, 1.0f);
	//vertices[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);


	vertices[3].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	//vertices[3].color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	vertices[2].texture = XMFLOAT2(0.0f, 0.5f);

	vertices[4].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	//vertices[4].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	vertices[2].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[5].position = XMFLOAT3(1.0f, -1.0f, 1.0f);
	//vertices[5].color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	vertices[2].texture = XMFLOAT2(0.5f, 0.5f);

	vertices[6].position = XMFLOAT3(1.0f, -1.0f, -1.0f);
	//vertices[6].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[2].texture = XMFLOAT2(0.5f, 1.0f);

	vertices[7].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	//vertices[7].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	vertices[2].texture = XMFLOAT2(1.0f, 0.5f);

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

bool model::initializeIndexBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	HRESULT result;
	m_indexCount = 36;

	int i;
	// Create the vertex array.
	indices.assign(m_indexCount, i);
	//indices = new unsigned long[m_indexCount];
	/*if (!indices)
	{
	OutputDebugString("Failed to create indecies");
	return false;
	}*/

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

void model::initializeInstance(int GridSize,int NumberOfModles)
{
	m_instanceCount = GridSize + NumberOfModles;

	InstanceType instance;
	instances.assign(m_instanceCount, instance);

	XMATRIXBufferType Matrix;
	instanceMatrixs.assign(m_instanceCount, Matrix);

	for (int i = 0; i < m_instanceCount; i++)
	{
		//initlise all postions to 0
		instances[i].InstanceMatrix = XMMatrixIdentity();
		instanceMatrixs[i].t = numeric_limits<double>::infinity();
 	}
}

bool model::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	// Create the texture object.
	m_Texture = new texture;
	if (!m_Texture)
	{
		return false;
	}

	if (!m_Texture->Initialize(device, filename))
	{
		return false;
	}

	return true;
}

#pragma endregion

void model::Render(float dt)
{
	return;
}

void model::Update(float dt)
{
	return;
}

void model::addInstance(int i,XMFLOAT3 postion, XMFLOAT3 Scale, XMFLOAT3 Rotation)
{

	XMMATRIX m_fudge = XMMatrixIdentity();

	instanceMatrixs[i].scale = Scale;
	XMMATRIX scaleMatrix = XMMatrixScaling(instanceMatrixs[i].scale.x, instanceMatrixs[i].scale.y, instanceMatrixs[i].scale.z);
	
	instanceMatrixs[i].rotaion = Rotation;
	XMMATRIX rotaionMatrix = XMMatrixRotationRollPitchYaw(instanceMatrixs[i].rotaion.x, instanceMatrixs[i].rotaion.y, instanceMatrixs[i].rotaion.z);
	
	instanceMatrixs[i].postion = postion;
	XMMATRIX trasformMatrix = XMMatrixTranslation(instanceMatrixs[i].postion.x, instanceMatrixs[i].postion.y, instanceMatrixs[i].postion.z);

	instances[i].InstanceMatrix = XMMatrixTranspose(m_fudge * scaleMatrix
		* rotaionMatrix * trasformMatrix);
}

int model::checkCollison(XMVECTOR rayDirc, XMVECTOR rayOrgin)
{
	int lowTInst = instances.size()-1;

	for (int i = 0; i < m_instanceCount; i++)
	{
		//set up bounding box 
		InstanceType instance = instances[i];
		XMATRIXBufferType instData = instanceMatrixs[i];

		//transfom vertices 
		XMFLOAT3 max = XMFLOAT3(instData.postion.x + instData.scale.x, instData.postion.y + instData.scale.y, instData.postion.z + instData.scale.z);
		
		XMFLOAT3 min = XMFLOAT3(instData.postion.x - instData.scale.x, instData.postion.y - instData.scale.y, instData.postion.z - instData.scale.z);

		XMVECTOR transfromVec;
		FXMVECTOR TRVec = XMVectorSet(max.x, max.y, max.z, 1.0f);
		FXMVECTOR LBVec = XMVectorSet(min.x, min.y, min.z, 1.0f);

		transfromVec = XMVector3Transform(TRVec, instance.InstanceMatrix);
		XMFLOAT3 TopRight = XMFLOAT3(XMVectorGetByIndex(transfromVec, 0), XMVectorGetByIndex(transfromVec, 1), XMVectorGetByIndex(transfromVec, 2));

		transfromVec = XMVector3Transform(LBVec, instance.InstanceMatrix);
		XMFLOAT3 LeftBottom = XMFLOAT3(XMVectorGetByIndex(transfromVec, 0), XMVectorGetByIndex(transfromVec, 1), XMVectorGetByIndex(transfromVec, 2));

		boundingBox->setBox(LeftBottom, TopRight);//each instance should have thereown bounding box instead
		//check for collison
		instData.t = boundingBox->rayCollison(rayDirc, rayOrgin);
		instanceMatrixs[i] = instData;
		// check t value
		if (instData.t < instanceMatrixs[lowTInst].t)
		{
			lowTInst = i;
		}
	}
	if (instanceMatrixs[lowTInst].t == numeric_limits<double>::infinity())
	{
		return numeric_limits<double>::infinity();
	}
	return lowTInst;//not gonna work 
}

void model::moveTo(int instanceID, XMFLOAT3 goalPos)
{
	float speed = 0.5f;
	if (instanceMatrixs[instanceID].postion.x > goalPos.x)
	{
		instanceMatrixs[instanceID].postion.x -= speed;
	}
	else if (instanceMatrixs[instanceID].postion.x < goalPos.x)
	{
		instanceMatrixs[instanceID].postion.x += speed;
	}

	if (instanceMatrixs[instanceID].postion.y > goalPos.y)
	{
		instanceMatrixs[instanceID].postion.y -= speed;
	}
	else if (instanceMatrixs[instanceID].postion.y < goalPos.y)
	{
		instanceMatrixs[instanceID].postion.y += speed;
	}
	
	if (instanceMatrixs[instanceID].postion.z > goalPos.z)
	{
		instanceMatrixs[instanceID].postion.z -= speed;
	}
	else if (instanceMatrixs[instanceID].postion.z < goalPos.z)
	{
		instanceMatrixs[instanceID].postion.z += speed;
	}
	updateInstanceMatrix(instanceID);
}


#pragma region getters and setters

ID3D11ShaderResourceView* model::GetTexture()
{
	return m_Texture->GetTexture();
}


int model::getInstanceCount()
{
	return m_instanceCount;
}

int model::getVertexCount()
{
	return m_vertexCount;
}

XMFLOAT3 model::getVerticesPostion(int i)
{
	return vertices[i].position;
}

int model::getIndexCount()
{
	return m_indexCount;
}

int model::getIndex(int i)
{
	return indices[i];
}

XMFLOAT3 model::getInstancePos(int instanceID)
{
	return instanceMatrixs[instanceID].postion;
}

void model::updateInstancePos(int instanceID, float X, float Y, float Z)
{
	instanceMatrixs[instanceID].postion = { X, Y, Z };
	updateInstanceMatrix(instanceID);
}

void model::updateInstanceMatrix(int instanceID)
{
	// Load the instance array with data.
	XMMATRIX m_fudge = XMMatrixIdentity();

	XMMATRIX scaleMatrix = XMMatrixScaling(instanceMatrixs[instanceID].scale.x, instanceMatrixs[instanceID].scale.y, instanceMatrixs[instanceID].scale.z);

	XMMATRIX rotaionMatrix = XMMatrixRotationRollPitchYaw(instanceMatrixs[instanceID].rotaion.x, instanceMatrixs[instanceID].rotaion.y, instanceMatrixs[instanceID].rotaion.z);

	XMMATRIX trasformMatrix = XMMatrixTranslation(instanceMatrixs[instanceID].postion.x, instanceMatrixs[instanceID].postion.y, instanceMatrixs[instanceID].postion.z);

	instances[instanceID].InstanceMatrix = XMMatrixTranspose(m_fudge * scaleMatrix * rotaionMatrix * trasformMatrix);
}

XMMATRIX model::GetModelMatrix(int instanceID)
{
	return instances[instanceID].InstanceMatrix;
}

#pragma endregion

bool model::updateInstancesBuffer(ID3D11Device* device)
{
	m_instanceBuffer = 0;
	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;

	HRESULT result;

	
	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances.data();
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the instance buffer.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}
	
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

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	/* Set the index buffer to active in the input assembler so it can be rendered.*/
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	return;
}
