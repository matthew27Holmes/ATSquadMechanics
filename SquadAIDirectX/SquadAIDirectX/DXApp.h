#pragma once
#define WIN32_LEAN_AND_MEAN

//linkging
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <string>
#include <d3d11.h>
#include <directxmath.h>
#include <math.h> 
#include <D3DCompiler.h>
#include <fstream>
#include <Vector>
#include <array>
#include <algorithm>
#include <stdlib.h>
#include <time.h>  
#include <limits>
#include <chrono>
#include <ctime>
#include <iostream>


#include "DirectXMath.h"

#include "DXUtil.h"

//#include "InputController.h"


using namespace DirectX;
using namespace std;


class DXApp
{
public:
	DXApp(HINSTANCE hInstance);
	virtual ~DXApp(void);
	//main loop
	int Run();

	//frame work 
	virtual bool Init();
	virtual void Update(float dt)=0;
	virtual void Render(float dt)=0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


	void BeginScene(float, float, float, float);
	void EndScene();
	
	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	//math heper functions
	XMFLOAT3 addFloat3(XMFLOAT3 A, XMFLOAT3 B);
	XMFLOAT3 subtractFloat3(XMFLOAT3 A, XMFLOAT3 B);

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pImmediateContext;

protected:

	// WIN 32 at
	HWND m_hAppWnd;
	HINSTANCE m_hAppInstance;
	UINT m_ClientWidth;
	UINT m_ClientHeight;
	std::string m_AppTitle;
	DWORD m_WndStyle;

	//Dx Attr
	
	IDXGISwapChain* m_pSwapchain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	D3D_DRIVER_TYPE m_DriverType;
	D3D_FEATURE_LEVEL m_FeatureLevel;
	D3D11_VIEWPORT m_Viewport;

	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;

	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;

	float screenNear = 0.1f, screenDepth= 1000.0f;
	int mouseX = 0, mouseY = 0;

	//InputController* m_input;
	
protected:
	bool InitWindow();

	//initilse direct 3D
	bool InitDirect3D();

	double SysTime;
	int frames = 0;
};

