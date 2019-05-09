#include "DXApp.h"

namespace
{
	//forward msg to user defined proc function
	DXApp* g_pApp = nullptr;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_pApp)
	{
		return g_pApp->MsgProc(hwnd,msg,wParam,lParam);
	}
	else
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}


DXApp::DXApp(HINSTANCE hInstance)
{
	m_hAppInstance = hInstance;
	m_hAppWnd = NULL;
	m_ClientWidth = 800;
	m_ClientHeight = 600;
	m_AppTitle = "Squad AI";
	m_WndStyle = WS_OVERLAPPEDWINDOW;
	g_pApp = this;

	m_pDevice = nullptr;
	m_pImmediateContext = nullptr;
	m_pRenderTargetView = nullptr;
	m_pSwapchain = nullptr;
}


DXApp::~DXApp()
{

	//clean up
	if (m_pImmediateContext)
	{
		m_pImmediateContext->ClearState();
	}
	Memory::SafeRelease(m_pRenderTargetView);
	Memory::SafeRelease(m_pSwapchain);
	Memory::SafeRelease(m_pImmediateContext);
	Memory::SafeRelease(m_pDevice);
}

int DXApp :: Run()
{
	MSG msg = { 0 };
	

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			auto start = std::chrono::system_clock::now();
			Update(0.0f);
			//render
			Render(0.0f);
			auto end = std::chrono::system_clock::now();

			std::chrono::duration<double> elapsed_seconds = end - start;

			SysTime += elapsed_seconds.count();
			frames++;
			if (SysTime > 1)
			{
				OutputDebugString((to_string(frames) + " , ").c_str());
				frames = 0;
				SysTime = -1;
			}
		}
	}
	return static_cast<int>(msg.wParam);
}

bool DXApp::Init()
{
	if (!InitWindow())
	{
		return false;
	}
	if (!InitDirect3D())
	{
		return false;
	}
	return true;
}

bool DXApp::InitWindow()
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = m_hAppInstance;
	wcex.lpfnWndProc = MainWndProc;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);//can call make int resurces custom icon
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "DXAPPWNDCLASS";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		OutputDebugString("\nFAILED TO CREATE WINDOW CLASS");
		return false;
	}
	 
	RECT r = {0, 0, m_ClientWidth, m_ClientHeight};
	AdjustWindowRect(&r, m_WndStyle, FALSE);
	UINT width = r.right - r.left;
	UINT height = r.bottom- r.top;
	
	UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

	m_hAppWnd = CreateWindow("DXAPPWNDCLASS", m_AppTitle.c_str(), m_WndStyle,
		x, y, width, height, NULL, NULL, m_hAppInstance, NULL);
	if (!m_hAppWnd)
	{
		OutputDebugString("\nFAILED TO CREATE WINDOW");
		return false;
	}

	ShowWindow(m_hAppWnd, SW_SHOW);
	return true;
}

bool DXApp::InitDirect3D()
{
	UINT createDeviceFlags = 0;

//#ifdef _DEBUG
//	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif // DEBUG


	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	//swap chain buffers

	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = m_ClientWidth;
	swapDesc.BufferDesc.Height = m_ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = m_hAppWnd;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = true;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//alt enter full screen

	HRESULT result;
	for (int i = 0; i < numDriverTypes; i++)
	{
		result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[i], NULL, 0,featureLevels,numFeatureLevels, 
			D3D11_SDK_VERSION, &swapDesc, &m_pSwapchain, &m_pDevice,&m_FeatureLevel, &m_pImmediateContext);

		if (SUCCEEDED(result))
		{
			m_DriverType = driverTypes[i];
			break;
		}
	}

	if (FAILED(result))
	{
		OutputDebugString("FAILED TO CREATE DEVICE AND SWAP CHAIN");
		return false;
	}

	// create render target view 
	ID3D11Texture2D* m_pbackBufferTex = 0;
	HR(m_pSwapchain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), 
		reinterpret_cast<void**>(&m_pbackBufferTex)));
	HR(m_pDevice->CreateRenderTargetView(m_pbackBufferTex, nullptr, &m_pRenderTargetView));
	Memory::SafeRelease(m_pbackBufferTex);

	// intitalize the descripiton of the depth buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	//set up the depth buffer desc
	depthBufferDesc.Width = m_ClientWidth;
	depthBufferDesc.Height = m_ClientHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//create the texture for the depth buffer using the desc
	result = m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//create depth stencil state 
	result = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	//set depth stencil state 
	m_pImmediateContext->OMSetDepthStencilState(m_depthStencilState, 1);

	//initilize depth stenicl view 
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//set up the depth view desc
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//create depth stencil view 
	result = m_pDevice->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	//bind
	m_pImmediateContext->OMSetRenderTargets(1,&m_pRenderTargetView, m_depthStencilView);

	// set raster desc
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// create rasterizer state 
	result = m_pDevice->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	//set rasterizer state 
	m_pImmediateContext->RSSetState(m_rasterState);
	
	
	//viewport creation
	m_Viewport.Width = static_cast<float>(m_ClientWidth);
	m_Viewport.Height = static_cast<float>(m_ClientHeight);
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.MinDepth = 0;
	m_Viewport.MaxDepth = 1.0f;

	// bind viewport 
	m_pImmediateContext->RSSetViewports(1, &m_Viewport);

	// setup project matrix for 3d rendering 
	float fieldOfView = 3.141592654f / 4.0f; //0.4f*3.14f;
	float screenAspect = (float)m_ClientWidth / (float)m_ClientHeight;

	//create project matrix
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	//intilize world matrix
	m_worldMatrix = XMMatrixIdentity();
	
	//create orthographic projection matirx for 2d rendering 
	m_orthoMatrix = XMMatrixOrthographicLH((float)m_ClientWidth, (float)m_ClientHeight, screenNear, screenDepth);

	return true;
}

void DXApp::BeginScene(float red, float blue, float green, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);

	// Clear the depth buffer.
	m_pImmediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	return;
}

void DXApp::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	/*if (m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_pSwapchain->Present(1, 0);
	}
	else
	{*/
		// Present as fast as possible.
		m_pSwapchain->Present(1, 0);//m_pSwapchain->Present(0, 0);

	return;
}


void DXApp::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}


void DXApp::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}


void DXApp::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

XMFLOAT3 DXApp::addFloat3(XMFLOAT3 A, XMFLOAT3 B)
{
	A.x += B.x;
	A.y += B.y;
	A.z += B.z;
	return A;
}

XMFLOAT3 DXApp::subtractFloat3(XMFLOAT3 A, XMFLOAT3 B)
{
	A.x -= B.x;
	A.y -= B.y;
	A.z -= B.z;
	return A;
}




LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
		{
			return 0;
		}

		default:
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
			break;
		}
	}
	return LRESULT();
}
