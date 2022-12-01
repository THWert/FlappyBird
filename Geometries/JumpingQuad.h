#pragma once
#include "Quad.h"

class JumpingQuad : public Quad
{
public:
	JumpingQuad(long left, long top, long right, long bottom);
	~JumpingQuad();

	void Jump();
	void Update();
	void Render();

private:
	bool bJump = false;
	float vy = 0.0f; //중력 같은 역할
	float jumpAccel = -1.0f;
	float jumpSpeed = 10.0f;

	bool doonce = true;

	long bottom = 0;

	bool bStart = false;
	float elapsedTime = 0.0f;
	float duration = 5.0f;

	RECT timeRect = { 0, 0, 100, 40 };
};