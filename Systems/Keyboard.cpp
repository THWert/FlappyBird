#include "stdafx.h"
#include "Keyboard.h"

void Keyboard::Update()
{
	memcpy(keyOldState, keyState, sizeof(keyOldState));
	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyMap  , sizeof(keyMap));

	GetKeyboardState(keyState);
	// 256���� ���� Ű ���¸� ������ ���ۿ� ����

	for (DWORD i = 0; i < MAX_INPUT_KEY; i++)
	{
		byte key = keyState[i] & 0x80;
		keyState[i] = key ? 1 : 0;

		int oldState = keyOldState[i];
		int state    = keyState   [i];

		if      (oldState == 0 && state == 1)
			keyMap[i] = KEY_INPUT_STATUS_DOWN;
		else if (oldState == 1 && state == 0)
			keyMap[i] = KEY_INPUT_STATUS_UP;
		else if (oldState == 1 && state == 1)
			keyMap[i] = KEY_INPUT_STATUS_PRESS;
	}
	GetKeyState(0);
	// Ű�� ��ۻ��¸� �˾ƿ�
	// Ű�� ����������(DOWN), ����������(UP) ȣ��
}

Keyboard::Keyboard()
{
	ZeroMemory(keyState   , sizeof(keyState));
	ZeroMemory(keyOldState, sizeof(keyOldState));//���� ����� �ִ��� Ȯ���ϴ°� Old
	ZeroMemory(keyMap     , sizeof(keyMap));
}

Keyboard::~Keyboard() {}