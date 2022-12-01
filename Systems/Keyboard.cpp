#include "stdafx.h"
#include "Keyboard.h"

void Keyboard::Update()
{
	memcpy(keyOldState, keyState, sizeof(keyOldState));
	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyMap  , sizeof(keyMap));

	GetKeyboardState(keyState);
	// 256개의 가상 키 상태를 지정된 버퍼에 복사

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
	// 키의 토글상태를 알아옴
	// 키가 눌려졌을때(DOWN), 떨어졌을때(UP) 호출
}

Keyboard::Keyboard()
{
	ZeroMemory(keyState   , sizeof(keyState));
	ZeroMemory(keyOldState, sizeof(keyOldState));//누른 기록이 있는지 확인하는게 Old
	ZeroMemory(keyMap     , sizeof(keyMap));
}

Keyboard::~Keyboard() {}