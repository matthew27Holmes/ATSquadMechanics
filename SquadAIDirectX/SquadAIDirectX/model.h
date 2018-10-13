#pragma once

#include "DXApp.h"

class model: public DXApp
{

public:
	model(HINSTANCE hInstance);
	~model()override;
	bool Init(ID3D11Device*) ;
	void Render(float dt) override;
	void RenderBuffers(ID3D11DeviceContext*);
	

	void Update(float dt)override;

	void GetModelMatrix(XMMATRIX& nwModelMatrix);
	void setPos(float x, float y, float z);
	void setScale(float scalex, float sacley, float saclez);
	void setRotaion(float pitch, float yaw, float roll);

	int getVertexCount();
	int getInstanceCount();

private:


	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	struct InstanceType
	{
		XMFLOAT3 position;
		XMFLOAT3 rotation;
		XMFLOAT3 Scale;
	};

	typedef struct _constantBufferStruct {
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	} ConstantBufferStruct;//pretty sure this dosnt work 
	static_assert((sizeof(ConstantBufferStruct) % 16) == 0, 
		"Constant Buffer size must be 16-byte aligned");

	bool InitializeBuffers(ID3D11Device*);

	bool CreateCube(ID3D11Device* device);
	bool createInstances(ID3D11Device* device);
	bool createCubeVertices(ID3D11Device* device);

	ID3D11Buffer * m_vertexBuffer, *m_instanceBuffer,*m_pConstantBuffer, *m_indexBuffer;


	ConstantBufferStruct m_constantBufferData;
	int m_vertexCount, m_instanceCount, m_frameCount, m_indexCount;
	float degree;


	float m_posX, m_posY, m_posZ;
	float m_scaleX, m_scaleY, m_scaleZ;
	float m_roll, m_yaw, m_pitch;
	XMMATRIX m_fudge, modelMatrix, scaleMatrix, rotaionMatrix, trasformMatrix;
};

