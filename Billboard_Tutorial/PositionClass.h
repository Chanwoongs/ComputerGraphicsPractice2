#pragma once

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(XMFLOAT3&);
	void GetRotation(XMFLOAT3&);

	void SetFrameTime(float);

	void MoveLeft(bool);
	void MoveRight(bool);

private:
	XMFLOAT3 m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

	float m_frameTime = 0;
	float m_leftSpeed = 0;
	float m_rightSpeed = 0;
};