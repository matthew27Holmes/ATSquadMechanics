#include "model.h"

model::model(HINSTANCE hInstance) : DXApp(hInstance)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_instanceBuffer = 0;

	m_vertexCount = 8;
	boundingBox = new Collider();
	m_model = 0;
}

model::~model()
{
	Memory::SafeRelease(m_instanceBuffer);
	Memory::SafeRelease(m_vertexBuffer);
	Memory::SafeRelease(m_indexBuffer);
	Memory::SafeDelete(boundingBox);
	Memory::SafeDelete(m_model);
	
}

bool model::Init(ID3D11Device* device, const WCHAR* textureFilename, int NumberOfModles)//, int GridSize
{
	// Load in the model data,
	bool result;
	const char *s = "../SquadAIDirectX/Resource/cube.txt";
	result = LoadModel(s);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = initializeLoadedBuffer(device);
	if (!result)
	{
		return false;
	}

	initializeInstance(NumberOfModles);

	return true;
}

#pragma region initialize

bool model::initializeLoadedBuffer(ID3D11Device * device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}
	// Load the vertex array and index array with data.
	for (i = 0; i<m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
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
		return false;
	}

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
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void model::initializeInstance(int NumberOfModles)
{
	m_instanceCount = NumberOfModles;//GridSize +

	InstanceType instance;
	instances.assign(m_instanceCount, instance);

	XMATRIXBufferType Matrix;
	instanceMatrixs.assign(m_instanceCount, Matrix);

	for (int i = 0; i < m_instanceCount; i++)
	{
		//initlise all postions to 0
		instances[i].InstanceMatrix = XMMatrixIdentity();
		instances[i].textureID = 0;

		instanceMatrixs[i].t = numeric_limits<double>::infinity();
	}
}

bool model::LoadModel(const char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

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

void model::addInstance(int i,XMFLOAT3 postion, XMFLOAT3 Scale, XMFLOAT3 Rotation, int textureID)
{

	XMMATRIX m_fudge = XMMatrixIdentity();

	instanceMatrixs[i].scale = Scale;
	XMMATRIX scaleMatrix = XMMatrixScaling(instanceMatrixs[i].scale.x, instanceMatrixs[i].scale.y, instanceMatrixs[i].scale.z);
	
	instanceMatrixs[i].rotaion = Rotation;
	XMMATRIX rotaionMatrix = XMMatrixRotationRollPitchYaw(instanceMatrixs[i].rotaion.x, instanceMatrixs[i].rotaion.y, instanceMatrixs[i].rotaion.z);
	
	instanceMatrixs[i].postion = postion;
	XMMATRIX trasformMatrix = XMMatrixTranslation(instanceMatrixs[i].postion.x, instanceMatrixs[i].postion.y, instanceMatrixs[i].postion.z);

	instances[i].textureID = textureID;
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

void model::updateInstanceTexture(int instanceID, int textureID)
{
	instances[instanceID].textureID = textureID;
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

void model::RenderBuffers(ID3D11DeviceContext* deviceContext,textureShader* texture)
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

	texture->RenderShader(deviceContext, getIndexCount(), getVertexCount(), getInstanceCount());
	return;
}
