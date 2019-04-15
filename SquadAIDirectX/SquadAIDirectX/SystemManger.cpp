#include "SystemManger.h"

SystemManger::SystemManger(HINSTANCE hInstance) :DXApp(hInstance)
{
	//m_model = new model(hInstance);
	RTSGM = new RTSGameManger(hInstance);

	m_camera = new camera(hInstance);
	m_colourShader = new ColourShader(hInstance);
	m_input = new InputController();

	mouseX = 0, mouseY = 0;
	moveToPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

SystemManger:: ~SystemManger()
{
	Memory::SafeDelete(m_colourShader);
	//Memory::SafeDelete(m_model);
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
		return false;
	}

	m_camera->SetPosition(0.0f, 10.0f, -20.0f);
	m_camera->SetRotation(15.0f, 0.0f, 0.0f);

	/*if (!m_model->Init(m_pDevice))
	{
		OutputDebugString("Could not initialize the model object.");
		return false;
	}*/

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
		controlBoxPos();	
	}

	controlCamera();
	m_camera->Update(dt);

	/*m_model->Update(dt);
	m_model->updateInstancesBuffer(m_pDevice);*/
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

	//m_model->RenderBuffers(m_pImmediateContext);
	RTSGM->Render(dt, m_pImmediateContext);

	// Set the shader parameters used for rendering.
	m_colourShader->Render(dt);

	result = m_colourShader->SetShaderParameters(m_pImmediateContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		OutputDebugString("Falied to set shader parameters");
	}
	// render prepared buffers
	m_colourShader->RenderShader(m_pImmediateContext, RTSGM->getIndexCount(), RTSGM->getVertexCount(), RTSGM->getInstanceCount()); //m_model->getIndexCount(), m_model->getVertexCount(), m_model->getInstanceCount());

	DXApp::EndScene();
}


void SystemManger::controlBoxPos()
{
	//on click detection
	/* on click move to certain square using squad mechnanics 

	//CreateWorldRay();
	//int hitObjectId = m_model->checkCollison(rayDirection, rayOrigin);
	//if (hitObjectId != NULL)
	//{
		moveToPoint.y += 1.0f;
	//	m_model->moveTo(m_model->getInstanceCount()-3,m_model->getInstancePos(hitObjectId));// dont think y is doing anything 
	//}*/

	RTSGM->pathFind(1, moveToPoint);
	// find path for units 
	//then boids to move as a unit
}

#pragma region ray collison
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

/*
bool SystemManger::checkRayCollison()
{
	XMMATRIX modelMatrix;
	
	int instanceCount = m_model->getInstanceCount();
	for (int k = 0; k < instanceCount; k++)
	{
		if (m_model->getIsInstancesUnit(k) && m_model->getIsInstancesWalkable(k))
		{

			modelMatrix = m_model->GetModelMatrix(k);
			//Loop through each triangle in the object
			int indciesCount = m_model->getIndexCount();
			for (int i = 0; i < indciesCount / 3; i++)
			{
				//Triangle's vertices V1, V2, V3
				XMVECTOR tri1V1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				XMVECTOR tri1V2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				XMVECTOR tri1V3 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

				//Temporary 3d floats for each vertex
				XMFLOAT3 tV1, tV2, tV3;

				tV1 = m_model->getVerticesPostion(m_model->getIndex((i * 3) + 0));
				tV2 = m_model->getVerticesPostion(m_model->getIndex((i * 3) + 1));
				tV3 = m_model->getVerticesPostion(m_model->getIndex((i * 3) + 2));

				tri1V1 = { tV1.x, tV1.y, tV1.z ,0.0f };
				tri1V2 = { tV2.x, tV2.y, tV2.z,0.0f };
				tri1V3 = { tV3.x, tV3.y, tV3.z,0.0f };

				//Transform the vertices to world space
				tri1V1 = XMVector3Transform(tri1V1, modelMatrix);
				tri1V2 = XMVector3Transform(tri1V2, modelMatrix);
				tri1V3 = XMVector3Transform(tri1V3, modelMatrix);

				//Find the normal using U, V coordinates (two edges)
				XMVECTOR U = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				XMVECTOR V = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				XMVECTOR faceNormal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

				U = tri1V2 - tri1V1;
				V = tri1V3 - tri1V1;

				//Compute face normal by crossing U, V
				faceNormal = XMVector3Cross(U, V);

				faceNormal = XMVector3Normalize(faceNormal);

				//Calculate a point on the triangle for the plane equation
				XMVECTOR triPoint = tri1V1;

				//Get plane equation ("Ax + By + Cz + D = 0") Variables
				float tri1A = XMVectorGetX(faceNormal);
				float tri1B = XMVectorGetY(faceNormal);
				float tri1C = XMVectorGetZ(faceNormal);
				float tri1D = (-tri1A * XMVectorGetX(triPoint) - tri1B * XMVectorGetY(triPoint) - tri1C * XMVectorGetZ(triPoint));

				//Now we find where (on the ray) the ray intersects with the triangles plane
				float ep1, ep2, t = 0.0f;
				float planeIntersectX, planeIntersectY, planeIntersectZ = 0.0f;
				XMVECTOR pointInPlane = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

				ep1 = (XMVectorGetX(rayOrigin) * tri1A)
					+ (XMVectorGetY(rayOrigin) * tri1B)
					+ (XMVectorGetZ(rayOrigin) * tri1C);

				ep2 = (XMVectorGetX(rayDirection) * tri1A)
					+ (XMVectorGetY(rayDirection) * tri1B)
					+ (XMVectorGetZ(rayDirection) * tri1C);

				//Make sure there are no divide-by-zeros
				if (ep2 != 0.0f)
				{
					t = -(ep1 + tri1D) / (ep2);
				}

				if (t > 0.0f)    //Make sure you don't pick objects behind the camera
				{
					//Get the point on the plane
					planeIntersectX = XMVectorGetX(rayOrigin) + XMVectorGetX(rayDirection) * t;
					planeIntersectY = XMVectorGetY(rayOrigin) + XMVectorGetY(rayDirection) * t;
					planeIntersectZ = XMVectorGetZ(rayOrigin) + XMVectorGetZ(rayDirection) * t;

					pointInPlane = XMVectorSet(planeIntersectX, planeIntersectY, planeIntersectZ, 0.0f);

					if (PointInTriangle(tri1V1, tri1V2, tri1V3, pointInPlane))
					{
						//Return the distance to the hit, so you can check all the other pickable objects in your scene
						//and choose whichever object is closest to the camera
						//XMMATRIX modelTransform = m_model->getInstancePos(k);
						//pointInPlane = XMVector4Transform(pointInPlane, modelTransform);
						XMStoreFloat3(&moveToPoint, pointInPlane);
						return true;
						//return t / 2.0f;
					}
				}
			}
		}
	}
	return false;
}

bool SystemManger::PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point)
{
	//To find out if the point is inside the triangle, we will check to see if the point
	//is on the correct side of each of the triangles edges.
	
	//debug variables 
	XMFLOAT4 V1, V2, V3,c1,c2, PIP;
	XMStoreFloat4(&PIP, point);
	XMStoreFloat4(&V1, triV1);
	XMStoreFloat4(&V2, triV2);
	XMStoreFloat4(&V3, triV3);

	
	XMVECTOR cp1 = XMVector3Cross(XMVectorSubtract(triV3, triV2), XMVectorSubtract(point, triV2));
	XMVECTOR cp2 = XMVector3Cross(XMVectorSubtract(triV3 , triV2), XMVectorSubtract(triV1 , triV2));
	XMStoreFloat4(&c1, cp1);
	XMStoreFloat4(&c2, cp2);

	if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
	{
		cp1 = XMVector3Cross(XMVectorSubtract(triV3,triV1), XMVectorSubtract(point,triV1));
		cp2 = XMVector3Cross(XMVectorSubtract(triV3,triV1), XMVectorSubtract(triV2,triV1));
		XMStoreFloat4(&c1, cp1);
		XMStoreFloat4(&c2, cp2);
		if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
		{
			cp1 = XMVector3Cross(XMVectorSubtract(triV2,triV1), XMVectorSubtract(point,triV1));
			cp2 = XMVector3Cross(XMVectorSubtract(triV2 ,triV1), XMVectorSubtract(triV3,triV1));
			XMStoreFloat4(&c1, cp1);
			XMStoreFloat4(&c2, cp2);
			if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
			{
				return true;
			}
		}
	}
	return false;
}*/

#pragma endregion

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



