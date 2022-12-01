#include "stdafx.h"
#include "JumpingQuad.h"

JumpingQuad::JumpingQuad(long left, long top, long right, long bottom)
	:Quad(left, top, right, bottom) {}

JumpingQuad::~JumpingQuad() {}

void JumpingQuad::Jump()
{
	if (!doonce) return;
	doonce = false;
	bJump = true;
	vy = jumpSpeed;
	bottom = rect.bottom - 10;
}

void JumpingQuad::Update()
{
	//__super::Update();
	Quad::Update();

	if (elapsedTime >= duration)		//�ʹݿ� WASD�� �����°� �׽�Ʈ �ϱ� ���ؼ� �ð��� ����
	{
		if (bJump == true)
		{
			rect.bottom -= vy;
			rect.top -= vy;

			vy += jumpAccel;

			if (rect.bottom > bottom)	//������ ���� ������ ���̱��� ���� ������ false�� �ٲ�
			{
				bJump = false;
				doonce = true;
			}
		}
		else
		{
			rect.bottom += 3;
			rect.top += 3;
		}
	}
	elapsedTime += Timer::Delta();
}

void JumpingQuad::Render()
{
	Quad::Render();
	DrawText(memDC, to_wstring(vy).c_str(), -1, &timeRect, DT_CENTER | DT_VCENTER);
}