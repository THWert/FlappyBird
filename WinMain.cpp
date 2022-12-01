#include "stdafx.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command) //프로그램은 여기서 시작
{
	srand((uint)time(NULL));

	WinDesc desc;
	desc.AppName = L"WinAPI";
	desc.instance = instance;
	desc.handle = NULL;
	desc.width = WinMaxWidth;
	desc.height = WinMaxHeight;

	Window* window = new Window(desc); //윈도우 생성
	WPARAM wParam = window->Run(); //윈도우cpp 런 실행
	SAFE_DELETE(window);

	return wParam;
}