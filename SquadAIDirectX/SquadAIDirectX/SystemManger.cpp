#include "SystemManger.h"

SystemManger::SystemManger(HINSTANCE hInstance) :DXApp(hInstance)
{
	m_camera = new camera(hInstance);
	m_textureArray = new texture();
	m_textureShader = new textureShader(hInstance);

	m_input = new InputController();

	RTSGM = new RTSGameManger(hInstance);
	SquadLeader = -1;
	SquadMoved = false;
	mouseX = 0, mouseY = 0;
	moveToPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

SystemManger:: ~SystemManger()
{
	Memory::SafeDelete(m_textureShader);
	Memory::SafeDelete(RTSGM);
	Memory::SafeDelete(m_camera);
	Memory::SafeDelete(m_input);
}

bool SystemManger::Init()
{
	if (!DXApp::Init())
	{
		return false;
	}

	if (!m_input->Init(m_hAppInstance,m_hAppWnd,m_ClientWidth, m_ClientHeight))
	{
		OutputDebugString("Could not initialize input");
		return false;
	}

	m_camera->SetPosition(31.0f, 20.0f, 5.0f);
	m_camera->SetRotation(31.0f, -70.0f, 0.0f);

	if (!RTSGM->Init(m_pDevice))
	{
		OutputDebugString("Could not initialize the unit controler.");
		return false;
	}

	if (!m_textureShader->Init(m_pDevice))
	{
		OutputDebugString("Could not initialize the color shader object.");
		return false;
	}
	
	if (!m_textureArray->Initialize(m_pDevice, L"../SquadAIDirectX/Resource/seafloor.PNG", L"../SquadAIDirectX/Resource/Stone.PNG"
		, L"../SquadAIDirectX/Resource/BlueMetal.PNG", L"../SquadAIDirectX/Resource/RedMetal.PNG", L"../SquadAIDirectX/Resource/WallMetal.PNG", 
		L"../SquadAIDirectX/Resource/drum.PNG",L"../SquadAIDirectX/Resource/cardboard.PNG"))
	{
		OutputDebugString("Could not initialize the color shader object.");
		return false;
	}

	return true;
}

void SystemManger::Update(float dt)
{
	m_frameCount++;
	if (m_frameCount == MAXUINT)
	{
		m_frameCount = 0;
	}
	
	m_input->Update(m_hAppWnd);
	if (m_input->IsEscapePressed())
	{
		PostQuitMessage(WM_QUIT);
	}
	m_input->GetMouseLocation(mouseX, mouseY);
	

	if (m_input->IsLeftMouseButtonDown())
	{
		CreateWorldRay();
		int hitObjectId = 0;
		if (m_input->IsKeyDown(DIK_LCONTROL) || m_input->IsKeyDown(DIK_RCONTROL))
		{
			hitObjectId = RTSGM->checkShipCollison(rayDirection, rayOrigin);
			if (SquadMoved)
			{
				SquadMoved = false;
				SquadLeader = -1;
				RTSGM->deSelecteAll();
			}
			if (SquadLeader < 0)
			{
				SquadLeader = RTSGM->getUniteByUnitID(hitObjectId);
			}
			RTSGM->selectUnite(hitObjectId);
		}
		else
		{
			hitObjectId = RTSGM->checkTileCollison(rayDirection, rayOrigin);
			setSquadDestination(hitObjectId);
		}
	}

	controlCamera();
	m_camera->Update(dt);

	RTSGM->Update(dt, m_pDevice);

	m_textureShader->Update(dt);
}

void SystemManger::Render(float dt)
{
	bool result;

	// Clear the buffers to begin the scene.
	DXApp::BeginScene(0.529f, 0.980f, 0.807f, 1.0f);

	m_camera->Render(dt);
	DXApp::GetWorldMatrix(worldMatrix);
	m_camera->GetViewMatrix(viewMatrix);
	DXApp::GetProjectionMatrix(projectionMatrix);

	// Set the shader parameters used for rendering.
	m_textureShader->Render(dt);

	result = m_textureShader->SetShaderParameters(m_pImmediateContext, worldMatrix, viewMatrix, projectionMatrix,m_textureArray->GetTextureArray());
	if (!result)
	{
		OutputDebugString("Falied to set shader parameters");
	}
	// render prepared buffers
	RTSGM->Render(dt, m_pImmediateContext, m_textureShader);


	DXApp::EndScene();

	//OutputDebugString((to_string(elapsed_seconds.count())+" , ").c_str());

}

void SystemManger::setSquadDestination(int modelId)
{
	if (modelId >= 0 && modelId < RTSGM->getGridSize() && SquadLeader >= 0)
	{		
		RTSGM->findPath((int)SquadLeader, modelId);
		SquadMoved = true;
	}
}

#pragma region ray cast
void SystemManger::CreateWorldRay()
{
	XMVECTOR pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR pickRayInViewSpacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	float PRVecX, PRVecY, PRVecZ;


	XMFLOAT4X4 projectionFloat4;
	XMStoreFloat4x4(&projectionFloat4, projectionMatrix);

	//Transform 2D pick position on screen space to 3D ray in View space
	PRVecX = (((2.0f * mouseX) / m_ClientWidth) - 1) / projectionFloat4._11; 
	PRVecY = -(((2.0f * mouseY) / m_ClientHeight) - 1) / projectionFloat4._22;
	PRVecZ = 1;    //View space's Z direction ranges from 0 to 1, so we set 1 since the ray goes "into" the screen

	pickRayInViewSpaceDir = XMVectorSet(PRVecX, PRVecY, PRVecZ, 0.0f);

	//Uncomment this line if you want to use the center of the screen (client area)
	//to be the point that creates the picking ray (eg. first person shooter)
	//pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Transform 3D Ray from View space to 3D ray in World space
	XMMATRIX pickRayToWorldSpaceMatrix;
	XMVECTOR matInvDeter;    //We don't use this, but the xna matrix inverse function requires the first parameter to not be null

	pickRayToWorldSpaceMatrix = XMMatrixInverse(&matInvDeter, viewMatrix);    //Inverse of View Space matrix is World space matrix

	rayOrigin = XMVector3TransformCoord(pickRayInViewSpacePos, pickRayToWorldSpaceMatrix);
	rayDirection = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);
}
#pragma endregion

//need limits for the camera
void SystemManger::controlCamera()
{
	XMFLOAT3 currPos = m_camera->GetPosition();
	//control postion
	if (m_input->IsKeyDown(DIK_UPARROW))
	{
		m_camera->SetPosition(currPos.x, currPos.y, currPos.z + 1);
	}
	else if (m_input->IsKeyDown(DIK_DOWNARROW))
	{
		m_camera->SetPosition(currPos.x, currPos.y, currPos.z - 1);
	}

	if (m_input->IsKeyDown(DIK_RIGHTARROW))
	{
		m_camera->SetPosition(currPos.x + 0.3, currPos.y, currPos.z);
	}
	else if (m_input->IsKeyDown(DIK_LEFTARROW))
	{
		m_camera->SetPosition(currPos.x - 1, currPos.y, currPos.z);
	}

	if (m_input->IsKeyDown(DIK_Q))
	{
		m_camera->SetPosition(currPos.x, currPos.y + 1, currPos.z);
	}
	else if (m_input->IsKeyDown(DIK_E))
	{
		m_camera->SetPosition(currPos.x, currPos.y - 1, currPos.z);
	}

	XMFLOAT3 currRot = m_camera->GetRotation();
	//control rotaion
	if (m_input->IsKeyDown(DIK_A))
	{
		m_camera->SetRotation(currRot.x, currRot.y - 1, currRot.z);
	}
	else if (m_input->IsKeyDown(DIK_D))
	{
		m_camera->SetRotation(currRot.x, currRot.y + 1, currRot.z);
	}

	if (m_input->IsKeyDown(DIK_W))
	{
		m_camera->SetRotation(currRot.x - 1, currRot.y, currRot.z);
	}
	else if (m_input->IsKeyDown(DIK_S))
	{
		m_camera->SetRotation(currRot.x + 1, currRot.y, currRot.z);
	}
}



