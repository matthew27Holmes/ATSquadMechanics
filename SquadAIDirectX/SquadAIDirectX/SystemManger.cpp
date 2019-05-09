#include "SystemManger.h"

SystemManger::SystemManger(HINSTANCE hInstance) :DXApp(hInstance)
{
	m_camera = new camera(hInstance);
	m_colourShader = new ColourShader(hInstance);
	m_TextureShader = new textureShader();

	m_input = new InputController();

	RTSGM = new RTSGameManger(hInstance);
	SquadLeader = -1;
	SquadMoved = false;
	mouseX = 0, mouseY = 0;
	moveToPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

SystemManger:: ~SystemManger()
{
	Memory::SafeDelete(m_colourShader);
	Memory::SafeDelete(RTSGM);
	Memory::SafeDelete(m_camera);
	Memory::SafeDelete(m_input);
	Memory::SafeDelete(m_TextureShader);
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

	m_camera->SetPosition(0.0f, 10.0f, -20.0f);
	m_camera->SetRotation(15.0f, 0.0f, 0.0f);

	if (!RTSGM->Init(m_pDevice))
	{
		OutputDebugString("Could not initialize the unit controler.");
		return false;
	}

	if (!m_colourShader->Init(m_pDevice))
	{
		OutputDebugString("Could not initialize the color shader object.");
		return false;
	}

	if (!m_TextureShader->Initialize(m_pDevice, m_hAppWnd))
	{
		OutputDebugString("Could not initialize the texture shader object.");
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
		int hitObjectId = RTSGM->checkCollison(rayDirection, rayOrigin);
		if (m_input->IsKeyDown(DIK_LCONTROL) || m_input->IsKeyDown(DIK_RCONTROL))
		{

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
			setSquadDestination(hitObjectId);
		}
	}

	controlCamera();
	m_camera->Update(dt);

	RTSGM->Update(dt, m_pDevice);

	m_colourShader->Update(dt);
}

void SystemManger::Render(float dt)
{
	bool result;

	// Clear the buffers to begin the scene.
	DXApp::BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->Render(dt);
	DXApp::GetWorldMatrix(worldMatrix);
	m_camera->GetViewMatrix(viewMatrix);
	DXApp::GetProjectionMatrix(projectionMatrix);

	RTSGM->Render(dt, m_pImmediateContext);

	// Render the model using the texture shader.
	result = m_TextureShader->Render(m_pImmediateContext, RTSGM->getIndexCount(), RTSGM->getVertexCount(), RTSGM->getInstanceCount(), 
		worldMatrix, viewMatrix, projectionMatrix,RTSGM->GetTexture());
	if (!result)
	{
		OutputDebugString("Falied to set texture shader parameters");
	}

	//// Set the shader parameters used for rendering.
	//m_colourShader->Render(dt);

	//result = m_colourShader->SetShaderParameters(m_pImmediateContext, worldMatrix, viewMatrix, projectionMatrix);
	//if (!result)
	//{
	//	OutputDebugString("Falied to set shader parameters");
	//}
	//// render prepared buffers
	//m_colourShader->RenderShader(m_pImmediateContext, RTSGM->getIndexCount(), RTSGM->getVertexCount(), RTSGM->getInstanceCount()); //m_model->getIndexCount(), m_model->getVertexCount(), m_model->getInstanceCount());

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
		m_camera->SetPosition(currPos.x + 1, currPos.y, currPos.z);
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



