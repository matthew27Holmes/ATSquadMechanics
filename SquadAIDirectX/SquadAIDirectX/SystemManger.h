#pragma once

#include "model.h"
#include "camera.h"
#include "textureShader.h"
#include "DXApp.h"
#include "InputController.h"
#include "RTSGameManger.h" 
#include "texture.h"


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
	void setSquadDestination(int modelId);
	void CreateWorldRay();

	camera* m_camera;
	textureShader* m_textureShader;

	InputController* m_input;
	RTSGameManger* RTSGM;
	texture* m_textureArray;

	XMMATRIX viewMatrix, projectionMatrix, worldMatrix;;
	XMFLOAT3 moveToPoint;
	XMVECTOR rayOrigin, rayDirection;

	float m_frameCount;
	int mouseX, mouseY;
	int SquadLeader;
	bool SquadMoved;
	
};