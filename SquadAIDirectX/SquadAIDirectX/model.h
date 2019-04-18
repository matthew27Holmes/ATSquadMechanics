#pragma once

#include "DXApp.h"
#include "Collider.h"

class model: public DXApp
{
public:
	model(HINSTANCE hInstance);
	~model()override;
	bool Init(ID3D11Device*);
	void initializeInstance(int GridSize, int NumberOfModles);

	void Render(float dt) override;
	void RenderBuffers(ID3D11DeviceContext*);
	
	void Update(float dt)override;

	void addInstance(int i,XMFLOAT3 postion, XMFLOAT3 Scale, XMFLOAT3 Rotation);

	XMMATRIX GetModelMatrix(int instanceID);
	XMFLOAT3 getInstancePos(int instanceID);

	void updateInstancePos(int instanceID, float X,float Y,float Z);
	
	void updateInstanceMatrix(int instanceID);
	bool updateInstancesBuffer(ID3D11Device* device);

	void moveTo(int instanceID, XMFLOAT3 goalPos);
	int getVertexCount();
	XMFLOAT3 getVerticesPostion(int i);
	
	int getIndexCount();
	int getIndex(int i);
	int getInstanceCount();
	int checkCollison(XMVECTOR rayDirc, XMVECTOR rayOrgin);

private:

	//should add to instances buffer
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	struct InstanceType
	{
		XMMATRIX InstanceMatrix;
	};

	struct  XMATRIXBufferType 
	{
		XMFLOAT3 postion;
		XMFLOAT3 scale;
		XMFLOAT3 rotaion;
		float t; //collsion value
	};


	bool initializeCubeVertices(ID3D11Device* device);
	bool initializeIndexBuffer(ID3D11Device* device);
	
	vector<InstanceType> instances;
	vector<XMATRIXBufferType> instanceMatrixs;

	vector<VertexType> vertices;
	vector<int> indices;

	ID3D11Buffer *m_vertexBuffer, *m_instanceBuffer, *m_indexBuffer;
	Collider* boundingBox;

	int m_vertexCount, m_instanceCount, m_frameCount, m_indexCount;
	float degree;

};

