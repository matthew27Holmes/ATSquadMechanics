#pragma once

#include "DXApp.h"
#include <vector>
#include "Collider.h"

using namespace std;

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

	//bool getIsInstancesUnit(int instanceID);
	//bool getIsInstancesWalkable(int instanceID);

	void updateInstancePos(int instanceID, float X,float Y,float Z);
	
	//void updateInstanceIsUnit(int instanceID,bool unit);
	//void updateInstanceIsWalkable(int instanceID,bool walkable);

	void updateInstanceMatrix(int instanceID);
	bool updateInstancesBuffer(ID3D11Device* device);
	//bool pointBoxIntersecation(int id, XMFLOAT3 point);

	void moveTo(int instanceID, XMFLOAT3 goalPos);
	int getVertexCount();
	XMFLOAT3 getVerticesPostion(int i);//should this just return a single vertexs postion instead 
	
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
	};


	bool initializeCubeVertices(ID3D11Device* device);
	bool initializeIndexBuffer(ID3D11Device* device);
	
	vector<InstanceType> instances;
	vector<XMATRIXBufferType> instanceMatrixs;

	vector<VertexType> vertices;
	vector<int> indices;

	Collider* boundingBox;


	ID3D11Buffer * m_vertexBuffer, *m_instanceBuffer, *m_indexBuffer;

	int m_vertexCount, m_instanceCount, m_frameCount, m_indexCount;
	float degree;

};

