#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "SpecMapShaderClass.h"
#include "LightClass.h"
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Direct3D ??ü ????
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Direct3D ??ü ?ʱ?ȭ
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// m_Camera ??ü ????
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// ī?޶? ?????? ????
	XMMATRIX baseViewMatrix;
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// ???? ??ü ????
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// ???? ??ü ?ʱ?ȭ
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), "../Dx11Demo_21/data/cube.txt",	L"../Dx11Demo_21/data/stone02.dds",
													L"../Dx11Demo_21/data/bump02.dds", L"../Dx11Demo_21/data/spec02.dds"))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// specular map shader ??ü?? ?????Ѵ?.
	m_SpecMapShader = new SpecMapShaderClass;
	if(!m_SpecMapShader)
	{
		return false;
	}

	// specular map shader ??ü?? ?ʱ?ȭ?Ѵ?.
	if(!m_SpecMapShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the specular map shader object.", L"Error", MB_OK);
		return false;
	}

	// ???? ??ü?? ?????ϴ?.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// ???? ??ü?? ?ʱ?ȭ?մϴ?.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(16.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	// ???? ??ü?? ?????Ѵ?.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// specular map shader ??ü?? ?????Ѵ?.
	if(m_SpecMapShader)
	{
		m_SpecMapShader->Shutdown();
		delete m_SpecMapShader;
		m_SpecMapShader = 0;
	}

	// ???? ??ü ??ȯ
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// m_Camera ??ü ??ȯ
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D ??ü ??ȯ
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}


bool GraphicsClass::Frame()
{
	// ī?޶? ??ġ ????
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	return true;
}


bool GraphicsClass::Render()
{
	// ???? ?׸??? ???? ???۸? ?????ϴ?
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī?޶??? ??ġ?? ???? ?? ?????? ?????մϴ?
	m_Camera->Render();

	// ī?޶? ?? d3d ??ü???? ????, ?? ?? ???? ?????? ?????ɴϴ?
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// ?? ???????? rotation ?????? ??????Ʈ?մϴ?.
	static float rotation = 0.0f;
	rotation += (float)XM_PI * 0.0025f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// ȸ?? ?????? ???? ?????? ȸ???մϴ?.
	worldMatrix = XMMatrixRotationY(rotation);

	// ???? ???ؽ??? ?ε??? ???۸? ?׷??? ?????? ???ο? ??ġ?Ͽ? ?????? ?մϴ?.
	m_Model->Render(m_Direct3D->GetDeviceContext());


	// ???? ?? ???̴??? ?????Ͽ? ?????? ???????մϴ?.
	m_SpecMapShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
							m_Model->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), 
							m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	// ?????? ?????? ȭ?鿡 ?????մϴ?
	m_Direct3D->EndScene();

	return true;
}