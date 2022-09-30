////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_rotation.x = 0.0f;
	m_rotation.y = 0.0f;
	m_rotation.z = 0.0f;

	m_defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_moveLeftRight = 0.0f;
	m_moveBackForward = 0.0f;
	m_moveUpDown = 0.0f;

	m_yaw = 90.0f;
	m_pitch = 0.0f;
	m_roll = 0.0f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

void CameraClass::MoveLeft(float speed)
{
	m_moveLeftRight -= speed;
}

void CameraClass::MoveRight(float speed)
{
	m_moveLeftRight += speed;
}

void CameraClass::MoveBack(float speed)
{
	m_moveBackForward -= speed;
}

void CameraClass::MoveForward(float speed)
{
	m_moveBackForward += speed;
}

void CameraClass::MoveDown(float speed)
{
	m_moveUpDown -= speed;
}

void CameraClass::MoveUp(float speed)
{
	m_moveUpDown += speed;
}


XMFLOAT3 CameraClass::GetPosition()
{
	return m_position;
}


XMFLOAT3 CameraClass::GetRotation()
{
	return m_rotation;
}

// This uses the position and rotation of the camera to build and to update the view matrix.
void CameraClass::Render()
{
	XMVECTOR position;

	XMMATRIX rotationtempMatrix;

	// Setup the position of the camera in the world.
	position = XMLoadFloat3(&m_position);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	m_pitch = m_rotation.x * 0.0174532925f;
	m_yaw = m_rotation.y * 0.0174532925f;
	m_roll = m_rotation.z * 0.0174532925f;

	camRotationMatrix = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll);
	m_target = XMVector3TransformCoord(m_defaultForward, camRotationMatrix);
	m_target = XMVector3Normalize(m_target);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationtempMatrix = XMMatrixRotationY(m_yaw);

	m_camRight = XMVector3TransformCoord(m_defaultRight, rotationtempMatrix); // 회전한 만큼 카메라의 오른쪽 벡터 설정
	m_up = XMVector3TransformCoord(m_up, rotationtempMatrix); // 회전한 만큼 카메라의 위쪽 벡터 설정
	m_camForward = XMVector3TransformCoord(m_defaultForward, rotationtempMatrix); // 회전한 만큼 카메라의 앞 벡터 설정

	position += m_moveLeftRight * m_camRight; // 좌우 위치 이동
	position += m_moveBackForward * m_camForward; // 앞뒤 위치 이동
	position += m_moveUpDown * m_up; // 앞뒤 위치 이동
	m_position.x = XMVectorGetX(position);
	m_position.y = XMVectorGetY(position);
	m_position.z = XMVectorGetZ(position);

	m_moveLeftRight = 0.0f; // 초기화
	m_moveBackForward = 0.0f; // 초기화
	m_moveUpDown = 0.0f;

	// Translate the rotated camera position to the location of the viewer.
	m_target = position + m_target;

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(position, m_target, m_up);

	return;
}

void CameraClass::Move(float speed)
{

}


void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

void CameraClass::RotateCamera(float x, float y, float z)
{
	m_rotation.x += x;
	m_rotation.y += y;
	m_rotation.z += z;
}