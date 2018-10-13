#pragma once

#include "DXApp.h"

class ColourShader:public DXApp
{
public:
	ColourShader(HINSTANCE hInstance);
	~ColourShader()override;
	
	bool Init(ID3D11Device*);
	void Render(float ds)override;
	void RenderShader(ID3D11DeviceContext*, int, int);
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void Update(float dt)override;
	

private:

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	bool InitializeShader(ID3D11Device*, const  WCHAR*, const WCHAR*);

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

