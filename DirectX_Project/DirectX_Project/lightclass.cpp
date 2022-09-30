////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightclass.h"


LightClass::LightClass()
{
	m_ambientToggle = 1.0f;
	m_diffuseToggle = 1.0f;
	m_specularToggle = 1.0f;
	m_fogToggle = 1.0f;
}


LightClass::LightClass(const LightClass& other)
{
}


LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}

void LightClass::SetPosition(float x, float y, float z)
{
	m_position = XMFLOAT4(x, y, z, 1.0f);
	return;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
	return;
}

void LightClass::SetToggleAmbient(float toggle)
{
	m_ambientToggle = toggle;
}

void LightClass::SetToggleDiffuse(float toggle)
{
	m_diffuseToggle = toggle;
}

void LightClass::SetToggleSpecular(float toggle)
{
	m_specularToggle = toggle;
}

void LightClass::SetToggleFog(float toggle)
{
	m_fogToggle = toggle;
}

XMFLOAT4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

XMFLOAT3 LightClass::GetDirection()
{
	return m_direction;
}

XMFLOAT4 LightClass::GetPosition()
{
	return m_position;
}

XMFLOAT4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}

float LightClass::GetAmbientToggle()
{
	return m_ambientToggle;
}

float LightClass::GetDiffuseToggle()
{
	return m_diffuseToggle;
}

float LightClass::GetSpecularToggle()
{
	return m_specularToggle;
}

float LightClass::GetFogToggle()
{
	return m_fogToggle;
}
