#pragma once

#include "model.h"
#include "camera.h"
#include "ColourShader.h"
#include "DXApp.h"
#include "InputController.h"
#include "RTSGameManger.h" 

class  SystemManger : public DXApp
{
public:
	SystemManger(HINSTANCE hInstance);
	~SystemManger() override;

	bool Init() override;
	void Update(float dt)override;
	void Render(float dt)override;

private:

	void controlCamera();
	void controlBoxPos();
	void CreateWorldRay();
	/*bool checkRayCollison();
	bool PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point);
*/

	//model* m_model;

	camera* m_camera;
	ColourShader* m_colourShader;
	InputController* m_input;
	RTSGameManger* RTSGM;

	XMMATRIX viewMatrix, projectionMatrix, worldMatrix;;
	XMFLOAT3 moveToPoint;
	XMVECTOR rayOrigin, rayDirection;

	float m_frameCount;
	int mouseX, mouseY;
};