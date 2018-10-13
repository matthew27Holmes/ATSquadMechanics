#include "model.h"
#include "camera.h"
#include "ColourShader.h"
#include "DXApp.h"

class  SystemManger : public DXApp
{
public:
	 SystemManger(HINSTANCE hInstance);
	~SystemManger() override;

	bool Init() override;
	void Update(float dt)override;
	void Render(float dt)override;


private:
	model* m_model;
	camera* m_camera;
	ColourShader* m_colourShader;
	InputController* m_input;
	float m_frameCount;

};

 SystemManger:: SystemManger(HINSTANCE hInstance):DXApp(hInstance)
{
	 m_model = new model(hInstance);
	 m_camera = new camera(hInstance);
	 m_colourShader = new ColourShader(hInstance);
	 m_input = new InputController();
}

 SystemManger::~ SystemManger()
{
	Memory::SafeDelete(m_colourShader);
	Memory::SafeDelete(m_model);
	Memory::SafeDelete(m_camera);
}

 bool SystemManger::Init()
 {
	 if (!DXApp::Init())
	 {
		 return false;
	 }

	 if (!m_input->Initialize())
	 {
		 return false;
	 }

	 m_camera->SetPosition(0.0f, 0.0f, -10.0f);
	 m_model->setScale(1.0f, 1.0f, 1.0f);
	 m_model->setPos(0.0f, 0.0f, 0.0f);

	 if (!m_model->Init(m_pDevice))
	 {
 		 OutputDebugString("Could not initialize the model object.");
		 return false;
	 }

	 if (!m_colourShader->Init(m_pDevice))
	 {
		 OutputDebugString("Could not initialize the color shader object.");
		 return false;
	 }
 }

 void SystemManger::Update(float dt)
 {
	  static float rotation = 0.0f;

	  m_frameCount++;
	  if (m_frameCount == MAXUINT)
	  {
		  m_frameCount = 0;
	  }

	 m_camera->Update(dt);
	 // Update the rotation variable each frame.
	 rotation += (float)XM_PI * 0.01f;
	 if (rotation > 360.0f)
	 {
		 rotation -= 360.0f;
	 }
	 m_model->setRotaion(0.0f, rotation, 0.0f);
	 m_model->Update(dt);
	 m_colourShader->Update(dt);
 }

 void SystemManger::Render(float dt)
 {
	 XMMATRIX modelMatrix, viewMatrix, projectionMatrix;
	 bool result;

	 // Clear the buffers to begin the scene.
	 DXApp::BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	 m_camera->Render(dt);

	 m_camera->GetViewMatrix(viewMatrix);
	 DXApp::GetProjectionMatrix(projectionMatrix);

	 // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	 m_model->Render(dt);
	 m_model->RenderBuffers(m_pImmediateContext);
	 
	m_model->GetModelMatrix(modelMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	m_model->GetModelMatrix(modelMatrix);

	 // Set the shader parameters used for rendering.
	 m_colourShader->Render(dt);

	 result = m_colourShader->SetShaderParameters(m_pImmediateContext, modelMatrix, viewMatrix, projectionMatrix);
	 if (!result)
	 {
		 OutputDebugString("Falied to set shader parameters");
	 }

	 // render prepared buffers
	 m_colourShader->RenderShader(m_pImmediateContext, m_model->getVertexCount(), m_model->getInstanceCount());

	 DXApp::EndScene();
 }





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	SystemManger systemMan(hInstance);

	if (!systemMan.Init())
	{
		return 1;
	}
	return systemMan.Run();
}
