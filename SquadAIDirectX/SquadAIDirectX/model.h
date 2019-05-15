#pragma once

#include "DXApp.h"
#include "Collider.h"
#include "textureShader.h"
#include <fstream>


class model: public DXApp
{
public:
	model(HINSTANCE hInstance);
	~model()override;
	virtual bool Init(ID3D11Device*, const WCHAR*,int NumberOfModles);
	void initializeInstance(int NumberOfModles);

	bool LoadModel(const char*);

	void Render(float dt) override;
	void RenderBuffers(ID3D11DeviceContext*,textureShader*);
	
	void Update(float dt)override;

	void addInstance(int i,XMFLOAT3 postion, XMFLOAT3 Scale, XMFLOAT3 Rotation,int textureID);

	XMMATRIX GetModelMatrix(int instanceID);
	XMFLOAT3 getInstancePos(int instanceID);

	void updateInstancePos(int instanceID, float X,float Y,float Z);
	void updateInstanceTexture(int instanceID, int textureID);
	
	void updateInstanceMatrix(int instanceID);
	bool updateInstancesBuffer(ID3D11Device* device);

	void moveTo(int instanceID, XMFLOAT3 goalPos);
	int getVertexCount();
	XMFLOAT3 getVerticesPostion(int i);
	
	int getIndexCount();
	int getIndex(int i);
	int getInstanceCount();
	int checkCollison(XMVECTOR rayDirc, XMVECTOR rayOrgin);

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct InstanceType
	{
		XMMATRIX InstanceMatrix;
		int textureID;
	};

	struct  XMATRIXBufferType
	{
		XMFLOAT3 postion;
		XMFLOAT3 scale;
		XMFLOAT3 rotaion;
		float t;
	};

protected:

	bool initializeLoadedBuffer(ID3D11Device* device);
	
	vector<InstanceType> instances;
	vector<XMATRIXBufferType> instanceMatrixs;

	vector<VertexType> vertices;
	vector<int> indices;

	ID3D11Buffer *m_vertexBuffer, *m_instanceBuffer, *m_indexBuffer;
	Collider* boundingBox;
	ModelType* m_model;


	int m_vertexCount, m_instanceCount, m_frameCount, m_indexCount;
	float degree;

};

