#include "stdafx.h"
#include "Window.h"
#include "Program.h"
//�������� : ./
//�������� : ../
//����� : 
//������ : C:\, D:\, F:\ ���

WinDesc Window::desc;
Program* Window::program = nullptr;

Window::Window(WinDesc desc)
{
	WNDCLASSEX WndClass;
	{
		WndClass.cbClsExtra = 0;										//�� ������� ����
		WndClass.cbWndExtra = 0;										//�� ������� ����
		WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//��� ��
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//Ŀ�� ��� ����
		WndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);					//������ �ΰ�
		WndClass.hIconSm = WndClass.hIcon;								//���� Ŀ��
		WndClass.hInstance = desc.instance;								//� ���α׷����� Ȯ��
		WndClass.lpfnWndProc = (WNDPROC)WndProc;						//�޼��� ó�� �Լ��� �ּҰ� �� (�Լ� ������)
		WndClass.lpszClassName = desc.AppName.c_str();					//Ŭ���� �̸�
		WndClass.lpszMenuName = NULL;								    //�޴� �̸�
		WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;			//â ��Ÿ��
		WndClass.cbSize = sizeof(WNDCLASSEX);							//� �����츦 ����ؼ� �������
	}

	WORD wHr = RegisterClassEx(&WndClass);							//���ʿ��� ������ ������ ���
	assert(wHr != 0);
	//������ �߻��� �� ���� ���� �־�θ� ���α׷��� ������ �����ڿ��� �˷���
	//�ش� �Լ��� �ɸ��� ���� �߻���ġ, call stack �� ���� ������ �˰� ��

	desc.handle = CreateWindowEx									//������ ����
	(
		WS_EX_APPWINDOW,											//��Ÿ�� Ȯ��� �ɼ�
		desc.AppName.c_str(),										//Ŭ���� �̸�
		desc.AppName.c_str(),										//������ â �̸�
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,	//��Ÿ��
		CW_USEDEFAULT,												//������ â�� X��ǥ
		CW_USEDEFAULT,												//������ â�� Y��ǥ
		CW_USEDEFAULT,												//������ â�� ���� ũ��
		CW_USEDEFAULT,												//������ â�� ���� ũ��
		NULL,														//�θ�â�� �ڵ�
		(HMENU)NULL,												//�޴��� �ĺ���
		desc.instance,												//� ������ ���α׷����� �ĺ���
		NULL														//�Ⱦ�
	);

	RECT rect = { 0,0,(LONG)desc.width,(LONG)desc.height };			//�簢�� ��ǥ (LT, RB)
	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)desc.width) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)desc.height) / 2;
	//GetSystemMetrics : ��ǻ�Ϳ��� �ϵ��� ���� �޶����ų� ����ڿ� ���ؼ� �缳�� ������ �������� ���α׷����� �� ����� �� ���
	//SM_CXSCREEN : ���� ȭ�� �ػ��� X�� ũ��
	//SM_CYSCREEN : ���� ȭ�� �ػ��� Y�� ũ��

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);			//������ �� ũ�� ���� ����
	//x, y width, height
	MoveWindow
	(
		desc.handle,												//�ڵ�
		centerX, centerY,											//X, Y
		rect.right - rect.left, rect.bottom - rect.top,				//������ �ʺ�, ����
		true);

	ShowWindow(desc.handle, SW_SHOWNORMAL);							//������ â ���
	SetForegroundWindow(desc.handle);								//�ֻ��� ȭ�� ����
	//SetFocus(desc.handle);										//������ ��Ŀ�� ����

	ShowCursor(true);
	Window::desc = desc;											//description ����
}

Window::~Window()
{
	DestroyWindow(desc.handle);
	UnregisterClass(desc.AppName.c_str(), desc.instance);
}

WPARAM Window::Run()
{
	MSG msg = { 0 }; //������

	Keyboard::Create();
	Mouse::Create();
	Timer::Create();

	Timer::Get()->Start();

	program = new Program();

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg); //msg ������ Ű���� �޽����� ��� ���� ��� Ű�� �����ϴ� ���ڸ� ����
			DispatchMessage(&msg);	//������ ���ν����� ����
		}
		else {}
	}
	Timer::Delete();
	Mouse::Delete();
	Keyboard::Delete();
	SAFE_DELETE(program);

	return msg.wParam;
}

HDC memDC;
LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	Mouse::Get()->InputProc(message, wParam, lParam);

	PAINTSTRUCT ps;
	HDC hdc;
	HBITMAP hBitmap, oldBitmap;
	static RECT crtRect;

	ZeroMemory(&ps, sizeof(PAINTSTRUCT));

	switch (message)
	{
	case WM_CREATE:
	{
		SetTimer(handle, 1, 0, NULL);
		break;
	}
	//������ ���°� ���ϸ� ȣ���
	case WM_PAINT:
		GetClientRect(handle, &crtRect);
		hdc = BeginPaint(handle, &ps);
		{
			memDC = CreateCompatibleDC(hdc);
			//�μ��� �־��� hdc�� �޸�DC�� ����
			//(ȣȯ�ȴ� = ���� ������ ���� = ������ ������ ���� DC������ ��Ʈ���� ������ �� �ִ�)
			hBitmap = CreateCompatibleBitmap(hdc, crtRect.right, crtRect.bottom);
			//�޸� DC�� �׸��� ���ؼ��� �ش� �Լ��� ������ ���ϴ� ũ���� ��Ʈ���� �����ؾ���
			oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
			//DC�� ����� GDI Object�� �ڵ� ���� ������ �� ���
			//GDI(Graphic Device Interface) : �׷��� ��ü�� ǥ���ϰ� ����ͳ� �����Ϳ� ���� �����ġ�� ����
			PatBlt(memDC, 0, 0, crtRect.right, crtRect.bottom, WHITENESS);
			//������ �簢 ������ ��� ä���ϵ� ���� DC�� ���õǾ��ִ� �귯�ÿ� ȭ���� ������ ������
			//�����꿡 ���� �� ������ �پ��ϰ� ȥ���� �� �ִ�.
			{
				if (Timer::Get() != nullptr) //nullptr�� ��µǸ� �ڵ尡 ������ ������ ������ ������ ���
					Timer::Get()->Print();   //FPS ���
				if (Mouse::Get() != nullptr)
					Mouse::Get()->Print();   //���콺 ��ǥ ���
				if (program != nullptr)
					program->Render();
			}
			BitBlt(hdc, 0, 0, crtRect.right, crtRect.bottom, memDC, 0, 0, SRCCOPY);
			//�̹����� ȭ�鿡 ���
			SelectObject(memDC, oldBitmap);
			DeleteObject(hBitmap);
			DeleteDC(memDC);
		}
		EndPaint(handle, &ps);
		break;

	case WM_TIMER:
	{
		if (Timer::Get() != nullptr)
			Timer::Get()->Update();
		if (Mouse::Get() != nullptr)
			Mouse::Get()->Update();
		if (Keyboard::Get() != nullptr)
			Keyboard::Get()->Update();
		if (program != nullptr)
			program->Update();

		InvalidateRect(desc.handle, nullptr, false);
		//������ â�� ������Ʈ ������ �簢���� �߰�
		break;
	}
}

	if (message == WM_CLOSE || message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}
/*
		hdc = BeginPaint(handle, &ps);
		TextOut(hdc, 100, 100, L"ABC", 3);
		EndPaint(handle, &ps);
		return 0;
*/