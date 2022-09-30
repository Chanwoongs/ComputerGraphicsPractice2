////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_FogShader = 0;
	m_Light1 = 0;
	m_Light2 = 0;
	m_Light3 = 0;
	m_Bitmap = 0;
	m_Text = 0;

	m_ParticleShader = 0;
	m_ParticleSystem = 0;
	m_ParticlePosition = 0;
	m_ParticleCount = 160;

	m_ambient = true;
	m_diffuse = true;
	m_specular = true;

	XMMATRIX unitMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	this->unitMatrix = unitMatrix;
	heroVillainDirection = 1;
	meteorDirection = 1;
	meteorState = 1;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	XMMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 2.0f, -5.0f);	

	/*
	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	*/

	SetModelsInfo();
	InitializeModels(hwnd);

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}


	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(0.0f, 0.0f, 0.0f, 0.0f);
	m_Light->SetDirection(1.0f, -1.0f, 0.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	// Create the fog shader object.
	m_FogShader = new FogShaderClass;
	if (!m_FogShader)
	{
		return false;
	}

	// Initialize the fog shader object.
	result = m_FogShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the fog shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the first light object.
	m_Light1 = new LightClass;
	if (!m_Light1)
	{
		return false;
	}

	// Initialize the first light object.
	m_Light1->SetDiffuseColor(0.3f, 0.0f, 1.0f, 1.0f);
	m_Light1->SetPosition(0.0f, 100.0f, 100.0f);

	// Create the second light object.
	m_Light2 = new LightClass;
	if (!m_Light2)
	{
		return false;
	}

	// Initialize the second light object.
	m_Light2->SetDiffuseColor(0.7f, 0.0f, 0.0f, 1.0f);
	m_Light2->SetPosition(0.0f, 200.0f, -350.0f);

	// Create the third light object.
	m_Light3 = new LightClass;
	if (!m_Light3)
	{
		return false;
	}

	// Initialize the third light object.
	m_Light3->SetDiffuseColor(0.9f, 0.9f, 0.9f, 1.0f);
	m_Light3->SetPosition(0.0f, 100.0f, 0.0f);

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/seafloor.dds", 256, 256);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the particle shader object.
	m_ParticleShader = new ParticleShaderClass;
	if (!m_ParticleShader)
	{
		return false;
	}

	// Initialize the particle shader object.
	result = m_ParticleShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the particle system object.
	m_ParticleSystem = new ParticleSystemClass[m_ParticleCount];
	if (!m_ParticleSystem)
	{
		return false;
	}

	for (int i = 0; i < m_ParticleCount; i++)
	{
		// Initialize the particle system object.
		result = m_ParticleSystem[i].Initialize(m_D3D->GetDevice(), L"./data/star.dds");
		if (!result)
		{
			return false;
		}
	}

	m_ParticlePosition = new XMFLOAT3[m_ParticleCount];

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_ParticlePosition[8 * i + j].x = -175.0f + (50.0f * j);
			m_ParticlePosition[8 * i + j].y = 25.0f;
			m_ParticlePosition[8 * i + j].z = 475.0f - (50.0f * i);
		}
	}

	return true;
}

void GraphicsClass::toggleAmbient()
{
	if (m_Light->GetAmbientToggle() == 0.0f)
	{
		m_Light->SetToggleAmbient(1.0f);
	}
	else
	{
		m_Light->SetToggleAmbient(0.0f);
	}
}

void GraphicsClass::toggleDiffuse()
{
	if (m_Light->GetDiffuseToggle() == 0.0f)
	{
		m_Light->SetToggleDiffuse(1.0f);
	}
	else
	{
		m_Light->SetToggleDiffuse(0.0f);
	}
}

void GraphicsClass::toggleSpecular()
{
	if (m_Light->GetSpecularToggle() == 0.0f)
	{
		m_Light->SetToggleSpecular(1.0f);
	}
	else
	{
		m_Light->SetToggleSpecular(0.0f);
	}
}

void GraphicsClass::toggleFog()
{
	if (m_Light->GetFogToggle() == 0.0f)
	{
		m_Light->SetToggleFog(1.0f);
	}
	else
	{
		m_Light->SetToggleFog(0.0f);
	}
}

CameraClass* GraphicsClass::GetCamera()
{
	return m_Camera;
}

void GraphicsClass::Shutdown()
{
	// Release the model object.
	for (auto object : m_models)
	{
		if (object.model)
		{
			object.model->Shutdown();
			delete object.model;
			object.model = 0;

			delete[] object.positions;
			object.positions = 0;
		}
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}
	if (m_Light1)
	{
		delete m_Light1;
		m_Light1 = 0;
	}
	if (m_Light2)
	{
		delete m_Light2;
		m_Light2 = 0;
	}
	if (m_Light3)
	{
		delete m_Light3;
		m_Light3 = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the fog shader object.
	if (m_FogShader)
	{
		m_FogShader->Shutdown();
		delete m_FogShader;
		m_FogShader = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}
	/*
	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}
	*/
	// Release the particle system object.
	if (m_ParticleSystem)
	{
		for (int i = 0; i < m_ParticleCount; i++)
		{
			m_ParticleSystem[i].Shutdown();
		}
		delete[] m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// Release the particle shader object.
	if (m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}

	// Release the particle shader object.
	if (m_ParticlePosition)
	{
		delete m_ParticlePosition;
		m_ParticlePosition = 0;
	}


	return;
}

bool GraphicsClass::Frame(float frameTime)
{
	bool result;

	static float rotation = 0.0f;
	static float timer = 0.0f;

	// Update the rotation variable each frame.
	rotation += XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	timer += 1.0f;

	// Run the frame processing for the particle system.
	for (int i = 0; i < m_ParticleCount / 2; i++)
	{
		m_ParticleSystem[i].SetParticleMode(0);
		m_ParticleSystem[i].Frame(frameTime, m_D3D->GetDeviceContext());
	}
	for (int i = m_ParticleCount / 2; i < m_ParticleCount; i++)
	{
		m_ParticleSystem[i].SetParticleMode(1);
		m_ParticleSystem[i].Frame(frameTime, m_D3D->GetDeviceContext());
	}

	// Render the graphics scene.
	result = Render(rotation, timer);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation, float& timer)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, translateMatrix;
	bool result;
	XMFLOAT4 diffuseColor[3];
	XMFLOAT4 lightPosition[3];
	float fogColor, fogStart, fogEnd;
	XMFLOAT3 cameraPosition, modelPosition;
	double angle;
	float billboardRotation;

	// Set the color of the fog to grey.
	fogColor = 0.5f;

	// Set the start and end of the fog.
	fogStart = 1000.0f;
	fogEnd = -1000.0f;

	// Create the diffuse color array from the four light colors.
	diffuseColor[0] = m_Light1->GetDiffuseColor();
	diffuseColor[1] = m_Light2->GetDiffuseColor();
	diffuseColor[2] = m_Light3->GetDiffuseColor();


	// Create the light position array from the four light positions.
	lightPosition[0] = m_Light1->GetPosition();
	lightPosition[1] = m_Light2->GetPosition();
	lightPosition[2] = m_Light3->GetPosition();

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// 3D Rendering
	auto tempWorldMatrix = worldMatrix;
	m_models.at(35).model->RotateVertices(m_D3D->GetDevice(), rotation);

	UpdateModelsTransformMatrix(timer);
	
	for (auto object : m_models)
	{
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		object.model->Render(m_D3D->GetDeviceContext());
		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), object.model->GetVertexCount(), object.model->GetInstanceCount(),
			tempWorldMatrix * XMMatrixScaling(object.scale.x, object.scale.y, object.scale.z) * object.transformMatrix,
			viewMatrix, projectionMatrix,
			object.model->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
			m_Light->GetAmbientToggle(), m_Light->GetDiffuseToggle(), m_Light->GetSpecularToggle(), m_Light->GetFogToggle(),
			diffuseColor, lightPosition, fogStart, fogEnd);
		if (!result)
		{
			return false;
		}
	}

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// 2D Rendering
	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	/*
	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}


	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 100, 100);
	if (!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}
	*/

	for (int i = 0; i < m_ParticleCount; i++)
	{
		/*
		// Set the position of the billboard model.
		modelPosition.x = 0.0f;
		modelPosition.y = 10.0f;
		modelPosition.z = 0.0f;
		*/

		// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
		angle = atan2(m_ParticlePosition[i].x - cameraPosition.x, m_ParticlePosition[i].z - cameraPosition.z) * (180.0 / XM_PI);

		// Convert rotation into radians.
		billboardRotation = m_Camera->GetRotation().y * 0.0174532925f;

		tempWorldMatrix = worldMatrix;

		// Setup the rotation the billboard at the origin using the world matrix.
		tempWorldMatrix *= XMMatrixRotationY(billboardRotation);

		// Setup the translation matrix from the billboard model.
		translateMatrix = XMMatrixTranslation(m_ParticlePosition[i].x, m_ParticlePosition[i].y, m_ParticlePosition[i].z);

		// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
		tempWorldMatrix = XMMatrixMultiply(tempWorldMatrix, translateMatrix);

		// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_ParticleSystem[i].Render(m_D3D->GetDeviceContext());

		// Render the model using the texture shader.
		result = m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_ParticleSystem[i].GetIndexCount(), tempWorldMatrix, viewMatrix, projectionMatrix,
			m_ParticleSystem[i].GetTexture());
		if (!result)
		{
			return false;
		}
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::UpdateModelsTransformMatrix(float &timer)
{
	if ((int)timer % 175 == 0)
	{
		if (meteorState == 1)
			meteorState = 2;
		else if (meteorState == 3)
			meteorState = 4;
	}
	if ((int)timer % 350 == 0)
	{
		heroVillainDirection *= -1;
		meteorDirection *= -1;
		
		if (meteorState == 2)
			meteorState = 3;
		else if (meteorState == 4)
			meteorState = 1;
		
		timer = 0;
	}
	// Heroes Marching
	for (vector<Model>::iterator it = m_models.begin() + 1; it != m_models.begin() + 13; it++)
	{
		(*it).transformMatrix *= XMMatrixTranslation(0.0f, 0.0f, 0.1f * -heroVillainDirection);
	}
	// Villains Marching
	for (vector<Model>::iterator it = m_models.begin() + 13; it != m_models.begin() + 22; it++)
	{
		(*it).transformMatrix *= XMMatrixTranslation(0.0f, 0.0f, 0.1f * heroVillainDirection);
	}

	// Meteor
	if (meteorState == 1)
	{
		m_models.at(35).transformMatrix *= XMMatrixTranslation(0.0f, 1.2f * meteorDirection, 0.0f);
	}
	else if (meteorState == 2)
	{
		m_models.at(35).transformMatrix *= XMMatrixTranslation(0.0f, -2.0f * meteorDirection, 2.5f * meteorDirection);
	}
	else if (meteorState == 3)
	{
		m_models.at(35).transformMatrix *= XMMatrixTranslation(0.0f, -2.0f * meteorDirection, 2.5f * meteorDirection);
	}
	else if (meteorState == 4)
	{
		m_models.at(35).transformMatrix *= XMMatrixTranslation(0.0f, 1.2f * meteorDirection, 0.0f);
	}
}

bool GraphicsClass::InitializeModels(HWND hwnd)
{
	bool result;

	// Plane
	m_models.push_back(m_Plane);
	if (!m_models.at(0).model)
	{
		return false;
	}
	result = m_models.at(0).model->Initialize(m_D3D->GetDevice(), L"./data/cube.obj", L"./data/Plane.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Single Heroes
	// Iron Man
	m_models.push_back(m_IronMan);
	if (!m_models.at(1).model)
	{
		return false;
	}
	result = m_models.at(1).model->Initialize(m_D3D->GetDevice(), L"./data/ironMan.obj", L"./data/ironMan.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Katarina
	m_models.push_back(m_Katarina);
	if (!m_models.at(2).model)
	{
		return false;
	}
	result = m_models.at(2).model->Initialize(m_D3D->GetDevice(), L"./data/Katarina.obj", L"./data/Katarina.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Deadpool
	m_models.push_back(m_Deadpool);
	if (!m_models.at(3).model)
	{
		return false;
	}
	result = m_models.at(3).model->Initialize(m_D3D->GetDevice(), L"./data/Deadpool.obj", L"./data/Deadpool.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Spiderman
	m_models.push_back(m_Spiderman);
	if (!m_models.at(4).model)
	{
		return false;
	}
	result = m_models.at(4).model->Initialize(m_D3D->GetDevice(), L"./data/Spiderman.obj", L"./data/Spiderman.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Spongebob
	m_models.push_back(m_Spongebob);
	if (!m_models.at(5).model)
	{
		return false;
	}
	result = m_models.at(5).model->Initialize(m_D3D->GetDevice(), L"./data/Spongebob.obj", L"./data/Spongebob.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Patrick
	m_models.push_back(m_Patrick);
	if (!m_models.at(6).model)
	{
		return false;
	}
	result = m_models.at(6).model->Initialize(m_D3D->GetDevice(), L"./data/Patrick.obj", L"./data/Patrick.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Robocop
	m_models.push_back(m_Robocop);
	if (!m_models.at(7).model)
	{
		return false;
	}
	result = m_models.at(7).model->Initialize(m_D3D->GetDevice(), L"./data/Robocop.obj", L"./data/Robocop.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Hulk
	m_models.push_back(m_Hulk);
	if (!m_models.at(8).model)
	{
		return false;
	}
	result = m_models.at(8).model->Initialize(m_D3D->GetDevice(), L"./data/Hulk.obj", L"./data/Hulk.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Multiple Heroes
	// Tank
	m_models.push_back(m_Tank);
	if (!m_models.at(9).model)
	{
		return false;
	}
	result = m_models.at(9).model->Initialize(m_D3D->GetDevice(), L"./data/Tank.obj", L"./data/TankSnow.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// GiantRobot
	m_models.push_back(m_GiantRobot);
	if (!m_models.at(10).model)
	{
		return false;
	}
	result = m_models.at(10).model->Initialize(m_D3D->GetDevice(), L"./data/GiantRobot.obj", L"./data/GiantRobot.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// HulkBuster
	m_models.push_back(m_HulkBuster);
	if (!m_models.at(11).model)
	{
		return false;
	}
	result = m_models.at(11).model->Initialize(m_D3D->GetDevice(), L"./data/HulkBuster.obj", L"./data/HulkBuster.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// RifleSoldier
	m_models.push_back(m_RifleSoldier);
	if (!m_models.at(12).model)
	{
		return false;
	}
	result = m_models.at(12).model->Initialize(m_D3D->GetDevice(), L"./data/RifleSoldier.obj", L"./data/RifleSoldier.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Single Villain
	// Diablo
	m_models.push_back(m_Diablo);
	if (!m_models.at(13).model)
	{
		return false;
	}
	result = m_models.at(13).model->Initialize(m_D3D->GetDevice(), L"./data/Diablo.obj", L"./data/Diablo.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Smoker
	m_models.push_back(m_Smoker);
	if (!m_models.at(14).model)
	{
		return false;
	}
	result = m_models.at(14).model->Initialize(m_D3D->GetDevice(), L"./data/Smoker.obj", L"./data/Smoker.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// AbominationHulk
	m_models.push_back(m_AbominationHulk);
	if (!m_models.at(15).model)
	{
		return false;
	}
	result = m_models.at(15).model->Initialize(m_D3D->GetDevice(), L"./data/AbominationHulk.obj", L"./data/AbominationHulk.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Venom
	m_models.push_back(m_Venom);
	if (!m_models.at(16).model)
	{
		return false;
	}
	result = m_models.at(16).model->Initialize(m_D3D->GetDevice(), L"./data/Venom.obj", L"./data/Venom.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Xuchilbara
	m_models.push_back(m_Xuchilbara);
	if (!m_models.at(17).model)
	{
		return false;
	}
	result = m_models.at(17).model->Initialize(m_D3D->GetDevice(), L"./data/Xuchilbara.obj", L"./data/Xuchilbara.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Multiple Villains
	// HeavyRobot
	m_models.push_back(m_HeavyRobot);
	if (!m_models.at(18).model)
	{
		return false;
	}
	result = m_models.at(18).model->Initialize(m_D3D->GetDevice(), L"./data/HeavyRobot.obj", L"./data/HeavyRobot.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Librarian
	m_models.push_back(m_Librarian);
	if (!m_models.at(19).model)
	{
		return false;
	}
	result = m_models.at(19).model->Initialize(m_D3D->GetDevice(), L"./data/Librarian.obj", L"./data/Librarian.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// GiantAlien
	m_models.push_back(m_GiantAlien);
	if (!m_models.at(20).model)
	{
		return false;
	}
	result = m_models.at(20).model->Initialize(m_D3D->GetDevice(), L"./data/GiantAlien.obj", L"./data/GiantAlien.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// AlienWarrior
	m_models.push_back(m_AlienWarrior);
	if (!m_models.at(21).model)
	{
		return false;
	}
	result = m_models.at(21).model->Initialize(m_D3D->GetDevice(), L"./data/AlienWarrior.obj", L"./data/AlienWarrior.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Environment
	// OldHouse
	m_models.push_back(m_OldHouseN);
	if (!m_models.at(22).model)
	{
		return false;
	}
	result = m_models.at(22).model->Initialize(m_D3D->GetDevice(), L"./data/OldHouse.obj", L"./data/OldHouse.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	m_models.push_back(m_OldHouseS);
	if (!m_models.at(23).model)
	{
		return false;
	}
	result = m_models.at(23).model->Initialize(m_D3D->GetDevice(), L"./data/OldHouse.obj", L"./data/OldHouse.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	m_models.push_back(m_OldHouseE);
	if (!m_models.at(24).model)
	{
		return false;
	}
	result = m_models.at(24).model->Initialize(m_D3D->GetDevice(), L"./data/OldHouse.obj", L"./data/OldHouse.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	m_models.push_back(m_OldHouseW);
	if (!m_models.at(25).model)
	{
		return false;
	}
	result = m_models.at(25).model->Initialize(m_D3D->GetDevice(), L"./data/OldHouse.obj", L"./data/OldHouse.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// TrafficSign
	m_models.push_back(m_TrafficSignN);
	if (!m_models.at(26).model)
	{
		return false;
	}
	result = m_models.at(26).model->Initialize(m_D3D->GetDevice(), L"./data/TrafficSign.obj", L"./data/TrafficSign.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	m_models.push_back(m_TrafficSignE);
	if (!m_models.at(27).model)
	{
		return false;
	}
	result = m_models.at(27).model->Initialize(m_D3D->GetDevice(), L"./data/TrafficSign.obj", L"./data/TrafficSign.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// DamagedDock
	m_models.push_back(m_DamagedDockN);
	if (!m_models.at(28).model)
	{
		return false;
	}
	result = m_models.at(28).model->Initialize(m_D3D->GetDevice(), L"./data/DamagedDock.obj", L"./data/DamagedDock.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	m_models.push_back(m_DamagedDockE);
	if (!m_models.at(29).model)
	{
		return false;
	}
	result = m_models.at(29).model->Initialize(m_D3D->GetDevice(), L"./data/DamagedDock.obj", L"./data/DamagedDock.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// DestroyedTank
	m_models.push_back(m_DestroyedTank);
	if (!m_models.at(30).model)
	{
		return false;
	}
	result = m_models.at(30).model->Initialize(m_D3D->GetDevice(), L"./data/DestroyedTank.obj", L"./data/DestroyedTank.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// DestroyedHouse
	m_models.push_back(m_DestroyedHouse);
	if (!m_models.at(31).model)
	{
		return false;
	}
	result = m_models.at(31).model->Initialize(m_D3D->GetDevice(), L"./data/DestroyedHouse.obj", L"./data/DestroyedHouse.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// SpaceShip
	m_models.push_back(m_Spaceship);
	if (!m_models.at(32).model)
	{
		return false;
	}
	result = m_models.at(32).model->Initialize(m_D3D->GetDevice(), L"./data/SpaceShip.obj", L"./data/SpaceShip.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Hospital
	m_models.push_back(m_Hospital);
	if (!m_models.at(33).model)
	{
		return false;
	}
	result = m_models.at(33).model->Initialize(m_D3D->GetDevice(), L"./data/Hospital.obj", L"./data/Hospital.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Building
	m_models.push_back(m_Building);
	if (!m_models.at(34).model)
	{
		return false;
	}
	result = m_models.at(34).model->Initialize(m_D3D->GetDevice(), L"./data/Building.obj", L"./data/Building.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	
	// Rock
	m_models.push_back(m_Rock);
	if (!m_models.at(35).model)
	{
		return false;
	}
	result = m_models.at(35).model->Initialize(m_D3D->GetDevice(), L"./data/Rock.obj", L"./data/Rock.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	
	return true;
}

XMFLOAT3 GraphicsClass::divideXMF3(XMFLOAT3 a, XMFLOAT3 b)
{
	XMFLOAT3 temp;
	temp.x = a.x / b.x;
	temp.y = a.y / b.y;
	temp.z = a.z / b.z;

	return temp;
}

void GraphicsClass::SetModelsInfo()
{
	// Single Hero
	m_IronMan.instanceCount = 1;
	m_IronMan.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_IronMan.positions = new XMFLOAT3[m_IronMan.instanceCount];
	m_IronMan.positions[0] = divideXMF3(XMFLOAT3(0.0f, 0.0f, 31.0f), m_IronMan.scale);
	m_IronMan.model = new ModelClass(m_IronMan.positions, m_IronMan.instanceCount, 0.0f);
	m_IronMan.transformMatrix = unitMatrix;

	m_Katarina.instanceCount = 1;
	m_Katarina.scale = XMFLOAT3(0.3f, 0.3f, 0.3f);
	m_Katarina.positions = new XMFLOAT3[m_Katarina.instanceCount];
	m_Katarina.positions[0] = divideXMF3(XMFLOAT3(2.0f, 0.0f, 32.0f), m_Katarina.scale);
	m_Katarina.model = new ModelClass(m_Katarina.positions, m_Katarina.instanceCount, 0.0f);
	m_Katarina.transformMatrix = unitMatrix;

	m_Deadpool.instanceCount = 1;
	m_Deadpool.scale = XMFLOAT3(0.01f, 0.01f, 0.01f);
	m_Deadpool.positions = new XMFLOAT3[m_Deadpool.instanceCount];
	m_Deadpool.positions[0] = divideXMF3(XMFLOAT3(-4.0f, 0.0f, 32.0f), m_Deadpool.scale);
	m_Deadpool.model = new ModelClass(m_Deadpool.positions, m_Deadpool.instanceCount, XM_PI / 2);
	m_Deadpool.transformMatrix = unitMatrix;

	m_Spiderman.instanceCount = 1;
	m_Spiderman.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Spiderman.positions = new XMFLOAT3[m_Spiderman.instanceCount];
	m_Spiderman.positions[0] = divideXMF3(XMFLOAT3(-2.0f, 0.0f, 32.0f), m_Spiderman.scale);
	m_Spiderman.model = new ModelClass(m_Spiderman.positions, m_Spiderman.instanceCount, XM_PI);
	m_Spiderman.transformMatrix = unitMatrix;

	m_Spongebob.instanceCount = 1;
	m_Spongebob.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Spongebob.positions = new XMFLOAT3[m_Spongebob.instanceCount];
	m_Spongebob.positions[0] = divideXMF3(XMFLOAT3(-6.0f, 0.0f, 33.0f), m_Spongebob.scale);
	m_Spongebob.model = new ModelClass(m_Spongebob.positions, m_Spongebob.instanceCount, 0.0f);
	m_Spongebob.transformMatrix = unitMatrix;

	m_Patrick.instanceCount = 1;
	m_Patrick.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Patrick.positions = new XMFLOAT3[m_Patrick.instanceCount];
	m_Patrick.positions[0] = divideXMF3(XMFLOAT3(6.0f, 0.0f, 33.0f), m_Patrick.scale);
	m_Patrick.model = new ModelClass(m_Patrick.positions, m_Patrick.instanceCount, 0.0f);
	m_Patrick.transformMatrix = unitMatrix;

	m_Robocop.instanceCount = 1;
	m_Robocop.scale = XMFLOAT3(0.5f, 0.5f, 0.5f);
	m_Robocop.positions = new XMFLOAT3[m_Robocop.instanceCount];
	m_Robocop.positions[0] = divideXMF3(XMFLOAT3(4.0f, 0.0f, 32.0f), m_Robocop.scale);
	m_Robocop.model = new ModelClass(m_Robocop.positions, m_Robocop.instanceCount, 0.0f);
	m_Robocop.transformMatrix = unitMatrix;

	m_Hulk.instanceCount = 1;
	m_Hulk.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Hulk.positions = new XMFLOAT3[m_Hulk.instanceCount];
	m_Hulk.positions[0] = divideXMF3(XMFLOAT3(0.0f, 0.0f, 35.0f), m_Hulk.scale);
	m_Hulk.model = new ModelClass(m_Hulk.positions, m_Hulk.instanceCount, 0.0f);
	m_Hulk.transformMatrix = unitMatrix;

	// Multiple Heroes
	m_Tank.instanceCount = 108;
	m_Tank.scale = XMFLOAT3(0.005f, 0.005f, 0.005f);
	m_Tank.positions = new XMFLOAT3[m_Tank.instanceCount];
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_Tank.positions[6 * i + j] = divideXMF3(XMFLOAT3(-181.0f + (10.0f * i), 0.0f, 197.0f - (20.0f * j)), m_Tank.scale);
		}
	}
	m_Tank.model = new ModelClass(m_Tank.positions, m_Tank.instanceCount, XM_PI / 2);
	m_Tank.transformMatrix = unitMatrix;

	m_GiantRobot.instanceCount = 72;
	m_GiantRobot.scale = XMFLOAT3(10.0f, 10.0f, 10.0f);
	m_GiantRobot.positions = new XMFLOAT3[m_GiantRobot.instanceCount];
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_GiantRobot.positions[6 * i + j] = divideXMF3(XMFLOAT3(19.5f + (15.0f * i), 0.0f, 193.0f - (19.0f * j)), m_GiantRobot.scale);
		}
	}
	m_GiantRobot.model = new ModelClass(m_GiantRobot.positions, m_GiantRobot.instanceCount, 0.0f);
	m_GiantRobot.transformMatrix = unitMatrix;

	m_HulkBuster.instanceCount = 69;
	m_HulkBuster.scale = XMFLOAT3(1.5f, 1.5f, 1.5f);
	m_HulkBuster.positions = new XMFLOAT3[m_HulkBuster.instanceCount];
	for (int i = 0; i < 23; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m_HulkBuster.positions[3 * i + j] = divideXMF3(XMFLOAT3(-110.0f + (10.0f * i), 0.0f, 71.0f - (10.0f * j)), m_HulkBuster.scale);
		}
	}
	m_HulkBuster.model = new ModelClass(m_HulkBuster.positions, m_HulkBuster.instanceCount, 0.0f);
	m_HulkBuster.transformMatrix = unitMatrix;

	m_RifleSoldier.instanceCount = 288;
	m_RifleSoldier.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_RifleSoldier.positions = new XMFLOAT3[m_RifleSoldier.instanceCount];
	for (int i = 0; i < 72; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_RifleSoldier.positions[2 * i + j] = divideXMF3(XMFLOAT3(-79.0f + (1.0f * i), 0.0f, 40.0f - (5.0f * j)), m_RifleSoldier.scale);
		}
	}
	for (int i = 0; i < 72; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_RifleSoldier.positions[2 * i + j + 144] = divideXMF3(XMFLOAT3(8.0f + (1.0f * i), 0.0f, 40.0f - (5.0f * j)), m_RifleSoldier.scale);
		}
	}
	m_RifleSoldier.model = new ModelClass(m_RifleSoldier.positions, m_RifleSoldier.instanceCount, 0.0f);
	m_RifleSoldier.transformMatrix = unitMatrix;
	
	// Single Villain
	m_Diablo.instanceCount = 1;
	m_Diablo.scale = XMFLOAT3(1.5f, 1.5f, 1.5f);
	m_Diablo.positions = new XMFLOAT3[m_Diablo.instanceCount];
	m_Diablo.positions[0] = divideXMF3(XMFLOAT3(0.0f, 0.0f, -52.0f), m_Diablo.scale);
	m_Diablo.model = new ModelClass(m_Diablo.positions, m_Diablo.instanceCount, XM_PI);
	m_Diablo.transformMatrix = unitMatrix;

	m_Smoker.instanceCount = 1;
	m_Smoker.scale = XMFLOAT3(0.08f, 0.08f, 0.08f);
	m_Smoker.positions = new XMFLOAT3[m_Smoker.instanceCount];
	m_Smoker.positions[0] = divideXMF3(XMFLOAT3(20.0f, 0.0f, -60.0f), m_Smoker.scale);
	m_Smoker.model = new ModelClass(m_Smoker.positions, m_Smoker.instanceCount, XM_PI);
	m_Smoker.transformMatrix = unitMatrix;

	m_AbominationHulk.instanceCount = 1;
	m_AbominationHulk.scale = XMFLOAT3(0.01f, 0.01f, 0.01f);
	m_AbominationHulk.positions = new XMFLOAT3[m_AbominationHulk.instanceCount];
	m_AbominationHulk.positions[0] = divideXMF3(XMFLOAT3(8.0f, 0.0f, -55.0f), m_AbominationHulk.scale);
	m_AbominationHulk.model = new ModelClass(m_AbominationHulk.positions, m_AbominationHulk.instanceCount, XM_PI);
	m_AbominationHulk.transformMatrix = unitMatrix;

	m_Venom.instanceCount = 1;
	m_Venom.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Venom.positions = new XMFLOAT3[m_Venom.instanceCount];
	m_Venom.positions[0] = divideXMF3(XMFLOAT3(-8.0f, 0.0f, -55.0f), m_Venom.scale);
	m_Venom.model = new ModelClass(m_Venom.positions, m_Venom.instanceCount, XM_PI);
	m_Venom.transformMatrix = unitMatrix;

	m_Xuchilbara.instanceCount = 1;
	m_Xuchilbara.scale = XMFLOAT3(0.05f, 0.05f, 0.05f);
	m_Xuchilbara.positions = new XMFLOAT3[m_Xuchilbara.instanceCount];
	m_Xuchilbara.positions[0] = divideXMF3(XMFLOAT3(-20.0f, 0.0f, -60.0f), m_Xuchilbara.scale);
	m_Xuchilbara.model = new ModelClass(m_Xuchilbara.positions, m_Xuchilbara.instanceCount, XM_PI);
	m_Xuchilbara.transformMatrix = unitMatrix;

	// Multiple Viallains
	m_HeavyRobot.instanceCount = 250;
	m_HeavyRobot.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_HeavyRobot.positions = new XMFLOAT3[m_HeavyRobot.instanceCount];
	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_HeavyRobot.positions[10 * i + j] = divideXMF3(XMFLOAT3(-82.0f + (3.0f * i), 0.0f, -80.0f - (5.0f * j)), m_HeavyRobot.scale);
		}
	}
	m_HeavyRobot.model = new ModelClass(m_HeavyRobot.positions, m_HeavyRobot.instanceCount, XM_PI);
	m_HeavyRobot.transformMatrix = unitMatrix;

	m_Librarian.instanceCount = 160;
	m_Librarian.scale = XMFLOAT3(0.02f, 0.02f, 0.02f);
	m_Librarian.positions = new XMFLOAT3[m_Librarian.instanceCount];
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_Librarian.positions[10 * i + j] = divideXMF3(XMFLOAT3(10.0f + (5.0f * i), 0.0f, -80.0f - (5.0f * j)), m_Librarian.scale);
		}
	}
	m_Librarian.model = new ModelClass(m_Librarian.positions, m_Librarian.instanceCount, XM_PI);
	m_Librarian.transformMatrix = unitMatrix;

	m_GiantAlien.instanceCount = 30;
	m_GiantAlien.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_GiantAlien.positions = new XMFLOAT3[m_GiantAlien.instanceCount];
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_GiantAlien.positions[2 * i + j] = divideXMF3(XMFLOAT3(-140.0f + (20.0f * i), 0.0f, -155.0f - (45.0f * j)), m_GiantAlien.scale);
		}
	}
	m_GiantAlien.model = new ModelClass(m_GiantAlien.positions, m_GiantAlien.instanceCount, XM_PI);
	m_GiantAlien.transformMatrix = unitMatrix;

	m_AlienWarrior.instanceCount = 28;
	m_AlienWarrior.scale = XMFLOAT3(0.01f, 0.01f, 0.01f);
	m_AlienWarrior.positions = new XMFLOAT3[m_AlienWarrior.instanceCount];
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_AlienWarrior.positions[2 * i + j] = divideXMF3(XMFLOAT3(-67.0f + (7.0f * i), 60.0f, -90.0f - (8.0f * j)), m_AlienWarrior.scale);
		}
	}
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_AlienWarrior.positions[2 * i + j + 14] = divideXMF3(XMFLOAT3(25.0f + (7.0f * i), 60.0f, -90.0f - (8.0f * j)), m_AlienWarrior.scale);
		}
	}
	m_AlienWarrior.model = new ModelClass(m_AlienWarrior.positions, m_AlienWarrior.instanceCount, -XM_PI / 2);
	m_AlienWarrior.transformMatrix = unitMatrix;

	// Environment
	m_Plane.instanceCount = 4000;
	m_Plane.scale = XMFLOAT3(10.0f, 0.1f, 10.0f);
	m_Plane.positions = new XMFLOAT3[m_Plane.instanceCount];
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			m_Plane.positions[100 * i + j] = divideXMF3(XMFLOAT3(-195.0f + (10.0f * i), 0.0f, 495.0f - (10.0f * j)), m_Plane.scale);
		}
	}
	m_Plane.model = new ModelClass(m_Plane.positions, m_Plane.instanceCount, 0.0f);
	m_Plane.transformMatrix = unitMatrix;

	m_OldHouseN.instanceCount = 2;
	m_OldHouseN.scale = XMFLOAT3(0.01f, 0.01f, 0.01f);
	m_OldHouseN.positions = new XMFLOAT3[m_OldHouseN.instanceCount];
	m_OldHouseN.positions[0] = divideXMF3(XMFLOAT3(-105.0f, 0.0f, -306.0f), m_OldHouseN.scale);
	m_OldHouseN.positions[1] = divideXMF3(XMFLOAT3(131.0f, 0.0f, -383.0f), m_OldHouseN.scale);
	m_OldHouseN.model = new ModelClass(m_OldHouseN.positions, m_OldHouseN.instanceCount, XM_PI);
	m_OldHouseN.transformMatrix = unitMatrix;

	m_OldHouseS.instanceCount = 2;
	m_OldHouseS.scale = XMFLOAT3(0.01f, 0.01f, 0.01f);
	m_OldHouseS.positions = new XMFLOAT3[m_OldHouseS.instanceCount];
	m_OldHouseS.positions[0] = divideXMF3(XMFLOAT3(-133.0f, 0.0f, -408.0f), m_OldHouseS.scale);
	m_OldHouseS.positions[1] = divideXMF3(XMFLOAT3(109.0f, 0.0f, -458.0f), m_OldHouseS.scale);
	m_OldHouseS.model = new ModelClass(m_OldHouseS.positions, m_OldHouseS.instanceCount, 0.0f);
	m_OldHouseS.transformMatrix = unitMatrix;

	m_OldHouseE.instanceCount = 2;
	m_OldHouseE.scale = XMFLOAT3(0.01f, 0.01f, 0.01f);
	m_OldHouseE.positions = new XMFLOAT3[m_OldHouseE.instanceCount];
	m_OldHouseE.positions[0] = divideXMF3(XMFLOAT3(-96.0f, 0.0f, -465.0f), m_OldHouseE.scale);
	m_OldHouseE.positions[1] = divideXMF3(XMFLOAT3(-11.0f, 0.0f, -368.0f), m_OldHouseE.scale);
	m_OldHouseE.model = new ModelClass(m_OldHouseE.positions, m_OldHouseE.instanceCount, -XM_PI / 2);
	m_OldHouseE.transformMatrix = unitMatrix;

	m_OldHouseW.instanceCount = 2;
	m_OldHouseW.scale = XMFLOAT3(0.01f, 0.01f, 0.01f);
	m_OldHouseW.positions = new XMFLOAT3[m_OldHouseW.instanceCount];
	m_OldHouseW.positions[0] = divideXMF3(XMFLOAT3(105.0f, 0.0f, -279.0f), m_OldHouseW.scale);
	m_OldHouseW.positions[1] = divideXMF3(XMFLOAT3(46.0f, 0.0f, -419.0f), m_OldHouseW.scale);
	m_OldHouseW.model = new ModelClass(m_OldHouseW.positions, m_OldHouseW.instanceCount, XM_PI / 2);
	m_OldHouseW.transformMatrix = unitMatrix;

	m_TrafficSignN.instanceCount = 4;
	m_TrafficSignN.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);
	m_TrafficSignN.positions = new XMFLOAT3[m_TrafficSignN.instanceCount];
	m_TrafficSignN.positions[0] = divideXMF3(XMFLOAT3(-15.0f, 0.55f, -279.0f), m_TrafficSignN.scale);
	m_TrafficSignN.positions[1] = divideXMF3(XMFLOAT3(21.0f, 0.55f, -322.0f), m_TrafficSignN.scale);
	m_TrafficSignN.positions[2] = divideXMF3(XMFLOAT3(6.0f, 0.55f, -292.0f), m_TrafficSignN.scale);
	m_TrafficSignN.positions[3] = divideXMF3(XMFLOAT3(67.0f, 0.55f, -266.0f), m_TrafficSignN.scale);
	m_TrafficSignN.model = new ModelClass(m_TrafficSignN.positions, m_TrafficSignN.instanceCount, 0.0f);
	m_TrafficSignN.transformMatrix = unitMatrix;

	m_TrafficSignE.instanceCount = 6;
	m_TrafficSignE.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);
	m_TrafficSignE.positions = new XMFLOAT3[m_TrafficSignE.instanceCount];
	m_TrafficSignE.positions[0] = divideXMF3(XMFLOAT3(-55.0f, 0.55f, -337.0f), m_TrafficSignE.scale);
	m_TrafficSignE.positions[1] = divideXMF3(XMFLOAT3(2.0f, 0.55f, -374.0f), m_TrafficSignE.scale);
	m_TrafficSignE.positions[2] = divideXMF3(XMFLOAT3(-24.0f, 0.55f, -337.0f), m_TrafficSignE.scale);
	m_TrafficSignE.positions[3] = divideXMF3(XMFLOAT3(54.0f, 0.55f, -358.0f), m_TrafficSignE.scale);
	m_TrafficSignE.positions[4] = divideXMF3(XMFLOAT3(-47.0f, 0.55f, -292.0f), m_TrafficSignE.scale);
	m_TrafficSignE.positions[5] = divideXMF3(XMFLOAT3(-52.0f, 0.55f, -268.0f), m_TrafficSignE.scale);
	m_TrafficSignE.model = new ModelClass(m_TrafficSignE.positions, m_TrafficSignE.instanceCount, XM_PI / 2);
	m_TrafficSignE.transformMatrix = unitMatrix;

	m_DamagedDockN.instanceCount = 5;
	m_DamagedDockN.scale = XMFLOAT3(0.0075f, 0.0075f, 0.0075f);
	m_DamagedDockN.positions = new XMFLOAT3[m_DamagedDockN.instanceCount];
	m_DamagedDockN.positions[0] = divideXMF3(XMFLOAT3(-29.0f, 0.55f, -304.0f), m_DamagedDockN.scale);
	m_DamagedDockN.positions[1] = divideXMF3(XMFLOAT3(9.0f, 0.55f, -337.0f), m_DamagedDockN.scale);
	m_DamagedDockN.positions[2] = divideXMF3(XMFLOAT3(-87.0f, 0.55f, -364.0f), m_DamagedDockN.scale);
	m_DamagedDockN.positions[3] = divideXMF3(XMFLOAT3(-136.0f, 0.55f, -327.0f), m_DamagedDockN.scale);
	m_DamagedDockN.positions[4] = divideXMF3(XMFLOAT3(-52.0f, 0.55f, -323.0f), m_DamagedDockN.scale);
	m_DamagedDockN.model = new ModelClass(m_DamagedDockN.positions, m_DamagedDockN.instanceCount, 0.0f);
	m_DamagedDockN.transformMatrix = unitMatrix;

	m_DamagedDockE.instanceCount = 6;
	m_DamagedDockE.scale = XMFLOAT3(0.0075f, 0.0075f, 0.0075f);
	m_DamagedDockE.positions = new XMFLOAT3[m_DamagedDockE.instanceCount];
	m_DamagedDockE.positions[0] = divideXMF3(XMFLOAT3(62.0f, 0.55f, -338.0f), m_DamagedDockE.scale);
	m_DamagedDockE.positions[1] = divideXMF3(XMFLOAT3(-147.0f, 0.55f, -379.0f), m_DamagedDockE.scale);
	m_DamagedDockE.positions[2] = divideXMF3(XMFLOAT3(-15.0f, 0.55f, -411.0f), m_DamagedDockE.scale);
	m_DamagedDockE.positions[3] = divideXMF3(XMFLOAT3(54.0f, 0.55f, -263.0f), m_DamagedDockE.scale);
	m_DamagedDockE.positions[4] = divideXMF3(XMFLOAT3(-46.0f, 0.55f, -254.0f), m_DamagedDockE.scale);
	m_DamagedDockE.positions[5] = divideXMF3(XMFLOAT3(-154.0f, 0.55f, -286.0f), m_DamagedDockE.scale);
	m_DamagedDockE.model = new ModelClass(m_DamagedDockE.positions, m_DamagedDockE.instanceCount, XM_PI / 2);
	m_DamagedDockE.transformMatrix = unitMatrix;

	m_DestroyedTank.instanceCount = 7;
	m_DestroyedTank.scale = XMFLOAT3(0.005f, 0.005f, 0.005f);
	m_DestroyedTank.positions = new XMFLOAT3[m_DestroyedTank.instanceCount];
	m_DestroyedTank.positions[0] = divideXMF3(XMFLOAT3(-134.0f, 0.55f, -267.0f), m_DestroyedTank.scale);
	m_DestroyedTank.positions[1] = divideXMF3(XMFLOAT3(-60.0f, 0.55f, -285.0f), m_DestroyedTank.scale);
	m_DestroyedTank.positions[2] = divideXMF3(XMFLOAT3(19.0f, 0.55f, -269.0f), m_DestroyedTank.scale);
	m_DestroyedTank.positions[3] = divideXMF3(XMFLOAT3(66.0f, 0.55f, -377.0f), m_DestroyedTank.scale);
	m_DestroyedTank.positions[4] = divideXMF3(XMFLOAT3(-59.0f, 0.55f, -390.0f), m_DestroyedTank.scale);
	m_DestroyedTank.positions[5] = divideXMF3(XMFLOAT3(-125.0f, 0.55f, -360.0f), m_DestroyedTank.scale);
	m_DestroyedTank.positions[6] = divideXMF3(XMFLOAT3(-37.0f, 0.55f, -449.0f), m_DestroyedTank.scale);
	m_DestroyedTank.model = new ModelClass(m_DestroyedTank.positions, m_DestroyedTank.instanceCount, -XM_PI / 2);
	m_DestroyedTank.transformMatrix = unitMatrix;

	m_DestroyedHouse.instanceCount = 7;
	m_DestroyedHouse.scale = XMFLOAT3(0.05f, 0.05f, 0.05f);
	m_DestroyedHouse.positions = new XMFLOAT3[m_DestroyedHouse.instanceCount];
	m_DestroyedHouse.positions[0] = divideXMF3(XMFLOAT3(-80.0f, 0.55f, -329.0f), m_DestroyedHouse.scale);
	m_DestroyedHouse.positions[1] = divideXMF3(XMFLOAT3(22.0f, 0.55f, -362.0f), m_DestroyedHouse.scale);
	m_DestroyedHouse.positions[2] = divideXMF3(XMFLOAT3(-8.0f, 0.55f, -269.0f), m_DestroyedHouse.scale);
	m_DestroyedHouse.positions[3] = divideXMF3(XMFLOAT3(66.0f, 0.55f, -310.0f), m_DestroyedHouse.scale);
	m_DestroyedHouse.positions[4] = divideXMF3(XMFLOAT3(-112.0f, 0.55f, -346.0f), m_DestroyedHouse.scale);
	m_DestroyedHouse.positions[5] = divideXMF3(XMFLOAT3(-80.0f, 0.55f, -262.0f), m_DestroyedHouse.scale);
	m_DestroyedHouse.positions[6] = divideXMF3(XMFLOAT3(-88.0f, 0.55f, -397.0f), m_DestroyedHouse.scale);
	m_DestroyedHouse.model = new ModelClass(m_DestroyedHouse.positions, m_DestroyedHouse.instanceCount, 0.0f);
	m_DestroyedHouse.transformMatrix = unitMatrix;

	m_Spaceship.instanceCount = 1;
	m_Spaceship.scale = XMFLOAT3(2.0f, 2.0f, 2.0f);
	m_Spaceship.positions = new XMFLOAT3[m_Spaceship.instanceCount];
	m_Spaceship.positions[0] = divideXMF3(XMFLOAT3(0.0f, 116.0f, -256.0f), m_Spaceship.scale);
	m_Spaceship.model = new ModelClass(m_Spaceship.positions, m_Spaceship.instanceCount, XM_PI);
	m_Spaceship.transformMatrix = unitMatrix;

	m_Hospital.instanceCount = 2;
	m_Hospital.scale = XMFLOAT3(0.02f, 0.02f, 0.02f);
	m_Hospital.positions = new XMFLOAT3[m_Hospital.instanceCount];
	m_Hospital.positions[0] = divideXMF3(XMFLOAT3(-128.0f, 0.0f, 370.0f), m_Hospital.scale);
	m_Hospital.positions[1] = divideXMF3(XMFLOAT3(128.0f, 0.0f, 370.0f), m_Hospital.scale);
	m_Hospital.model = new ModelClass(m_Hospital.positions, m_Hospital.instanceCount, 0.0f);
	m_Hospital.transformMatrix = unitMatrix;

	m_Building.instanceCount = 1;
	m_Building.scale = XMFLOAT3(0.01f, 0.01f, 0.01f);
	m_Building.positions = new XMFLOAT3[m_Building.instanceCount];
	m_Building.positions[0] = divideXMF3(XMFLOAT3(0.0f, 0.0f, 350.0f), m_Building.scale);
	m_Building.model = new ModelClass(m_Building.positions, m_Building.instanceCount, 0.0f);
	m_Building.transformMatrix = unitMatrix;

	m_Rock.instanceCount = 16;
	m_Rock.scale = XMFLOAT3(5.0f, 5.0f, 5.0f);
	m_Rock.positions = new XMFLOAT3[m_Rock.instanceCount];
	m_Rock.positions[0] = divideXMF3(XMFLOAT3(0.0f, 50.0f, -203.0f), m_Rock.scale);
	m_Rock.positions[1] = divideXMF3(XMFLOAT3(54.0f, 33.0f, -227.0f), m_Rock.scale);
	m_Rock.positions[2] = divideXMF3(XMFLOAT3(-33.0f, 22.0f, -236.0f), m_Rock.scale);
	m_Rock.positions[3] = divideXMF3(XMFLOAT3(-54.0f, 23.0f, -198.0f), m_Rock.scale);
	m_Rock.positions[4] = divideXMF3(XMFLOAT3(30.0f, 42.0f, -162.0f), m_Rock.scale);
	m_Rock.positions[5] = divideXMF3(XMFLOAT3(0.0f, 33.0f, -203.0f), m_Rock.scale);
	m_Rock.positions[6] = divideXMF3(XMFLOAT3(-34.0f, 38.0f, -227.0f), m_Rock.scale);
	m_Rock.positions[7] = divideXMF3(XMFLOAT3(-10.0f, 13.0f, -236.0f), m_Rock.scale);
	m_Rock.positions[8] = divideXMF3(XMFLOAT3(53.0f, 14.0f, -198.0f), m_Rock.scale);
	m_Rock.positions[9] = divideXMF3(XMFLOAT3(-19.0f, 19.0f, -162.0f), m_Rock.scale);
	m_Rock.positions[10] = divideXMF3(XMFLOAT3(-40.0f, 19.0f, -190.0f), m_Rock.scale);
	m_Rock.positions[11] = divideXMF3(XMFLOAT3(80.0f, 29.0f, -222.0f), m_Rock.scale);
	m_Rock.positions[12] = divideXMF3(XMFLOAT3(40.0f, 39.0f, -180.0f), m_Rock.scale);
	m_Rock.positions[13] = divideXMF3(XMFLOAT3(-80.0f, 21.0f, -190.0f), m_Rock.scale);
	m_Rock.positions[14] = divideXMF3(XMFLOAT3(-10.0f, 47.0f, -240.0f), m_Rock.scale);
	m_Rock.positions[15] = divideXMF3(XMFLOAT3(50.0f, 25.0f, -230.0f), m_Rock.scale);
	m_Rock.model = new ModelClass(m_Rock.positions, m_Rock.instanceCount, 0.0f);
	m_Rock.transformMatrix = unitMatrix;
}
