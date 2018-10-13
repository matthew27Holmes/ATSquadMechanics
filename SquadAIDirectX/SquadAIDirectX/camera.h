#pragma once
#include "DXApp.h"

class camera:public DXApp
{
public:
	camera(HINSTANCE hInstance);
	~camera()override;

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render(float dt) override;
	void Update(float dt) override;
	void GetViewMatrix(XMMATRIX&);

private:
	void cameraControls();
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix;
};

