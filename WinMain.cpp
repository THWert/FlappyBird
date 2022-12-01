#include "stdafx.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command) //���α׷��� ���⼭ ����
{
	srand((uint)time(NULL));

	WinDesc desc;
	desc.AppName = L"WinAPI";
	desc.instance = instance;
	desc.handle = NULL;
	desc.width = WinMaxWidth;
	desc.height = WinMaxHeight;

	Window* window = new Window(desc); //������ ����
	WPARAM wParam = window->Run(); //������cpp �� ����
	SAFE_DELETE(window);

	return wParam;
}