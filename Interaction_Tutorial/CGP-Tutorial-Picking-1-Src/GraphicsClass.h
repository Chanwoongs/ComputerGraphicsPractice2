#pragma once

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class D3DClass;
class InputClass;
class CameraClass;
class ModelClass;
class TextureShaderClass;
class BitmapClass;
class LightShaderClass;
class LightClass;
class TextClass;


class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool HandleInput();
	bool Render();
	void TestIntersection(int, int);
	bool RaySphereIntersect(XMFLOAT3, XMFLOAT3, float);

private:
	InputClass* m_Input = nullptr;
	D3DClass* m_D3D = nullptr;
	CameraClass* m_Camera = nullptr;
	ModelClass* m_Model = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
	LightShaderClass* m_LightShader = nullptr;
	LightClass* m_Light = nullptr;
	TextClass* m_Text = nullptr;
	BitmapClass* m_Bitmap = nullptr;
	bool m_beginCheck = false;
	int m_screenWidth = 0;
	int m_screenHeight = 0;
};