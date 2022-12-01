#include "stdafx.h"
#include "Window.h"
#include "Program.h"
//현재폴더 : ./
//상위폴더 : ../
//상대경로 : 
//절대경로 : C:\, D:\, F:\ 등등

WinDesc Window::desc;
Program* Window::program = nullptr;

Window::Window(WinDesc desc)
{
	WNDCLASSEX WndClass;
	{
		WndClass.cbClsExtra = 0;										//잘 사용하지 않음
		WndClass.cbWndExtra = 0;										//잘 사용하지 않음
		WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//배경 색
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//커서 모양 선택
		WndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);					//윈도우 로고
		WndClass.hIconSm = WndClass.hIcon;								//작은 커서
		WndClass.hInstance = desc.instance;								//어떤 프로그램인지 확인
		WndClass.lpfnWndProc = (WNDPROC)WndProc;						//메세지 처리 함수의 주소가 들어감 (함수 포인터)
		WndClass.lpszClassName = desc.AppName.c_str();					//클래스 이름
		WndClass.lpszMenuName = NULL;								    //메뉴 이름
		WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;			//창 스타일
		WndClass.cbSize = sizeof(WNDCLASSEX);							//어떤 윈도우를 사용해서 만드는지
	}

	WORD wHr = RegisterClassEx(&WndClass);							//위쪽에서 설정한 내용을 등록
	assert(wHr != 0);
	//오류가 발생할 것 같은 곳에 넣어두면 프로그램이 저절로 개발자에게 알려줌
	//해당 함수에 걸리면 버그 발생위치, call stack 등 여러 정보를 알게 됨

	desc.handle = CreateWindowEx									//윈도우 생성
	(
		WS_EX_APPWINDOW,											//스타일 확장된 옵션
		desc.AppName.c_str(),										//클래스 이름
		desc.AppName.c_str(),										//윈도우 창 이름
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,	//스타일
		CW_USEDEFAULT,												//윈도우 창의 X좌표
		CW_USEDEFAULT,												//윈도우 창의 Y좌표
		CW_USEDEFAULT,												//윈도우 창의 가로 크기
		CW_USEDEFAULT,												//윈도우 창의 세로 크기
		NULL,														//부모창이 핸들
		(HMENU)NULL,												//메뉴의 식별자
		desc.instance,												//어떤 윈도우 프로그램인지 식별자
		NULL														//안씀
	);

	RECT rect = { 0,0,(LONG)desc.width,(LONG)desc.height };			//사각형 좌표 (LT, RB)
	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)desc.width) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)desc.height) / 2;
	//GetSystemMetrics : 컴퓨터에서 하드웨어에 따라 달라지거나 사용자에 의해서 재설정 가능한 정보들을 프로그램에서 얻어서 사용할 때 사용
	//SM_CXSCREEN : 현재 화면 해상도의 X축 크기
	//SM_CYSCREEN : 현재 화면 해상도의 Y축 크기

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);			//윈도우 차 크기 조정 세팅
	//x, y width, height
	MoveWindow
	(
		desc.handle,												//핸들
		centerX, centerY,											//X, Y
		rect.right - rect.left, rect.bottom - rect.top,				//윈도우 너비, 높이
		true);

	ShowWindow(desc.handle, SW_SHOWNORMAL);							//윈도우 창 출력
	SetForegroundWindow(desc.handle);								//최상위 화면 설정
	//SetFocus(desc.handle);										//윈도우 포커스 조정

	ShowCursor(true);
	Window::desc = desc;											//description 설정
}

Window::~Window()
{
	DestroyWindow(desc.handle);
	UnregisterClass(desc.AppName.c_str(), desc.instance);
}

WPARAM Window::Run()
{
	MSG msg = { 0 }; //스니핑

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

			TranslateMessage(&msg); //msg 변수에 키보드 메시지가 들어 있을 경우 키에 대응하는 문자를 만들어냄
			DispatchMessage(&msg);	//윈도우 프로시저로 보냄
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
	//윈도우 상태가 변하면 호출됨
	case WM_PAINT:
		GetClientRect(handle, &crtRect);
		hdc = BeginPaint(handle, &ps);
		{
			memDC = CreateCompatibleDC(hdc);
			//인수로 주어진 hdc와 메모리DC를 생성
			//(호환된다 = 색상 포맷이 같다 = 색상이 포맷이 같은 DC끼리는 비트맵을 전송할 수 있다)
			hBitmap = CreateCompatibleBitmap(hdc, crtRect.right, crtRect.bottom);
			//메모리 DC에 그리기 위해서는 해당 함수로 생성한 원하는 크기의 비트맵을 선택해야함
			oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
			//DC에 저장된 GDI Object의 핸들 값을 변경할 때 사용
			//GDI(Graphic Device Interface) : 그래픽 객체를 표시하고 모니터나 프린터와 같은 출력장치로 전송
			PatBlt(memDC, 0, 0, crtRect.right, crtRect.bottom, WHITENESS);
			//지정된 사각 영역을 모두 채색하되 현재 DC에 선택되어있는 브러시와 화면의 색상을 논리연산
			//논리연산에 따라 두 색상을 다양하게 혼합할 수 있다.
			{
				if (Timer::Get() != nullptr) //nullptr이 출력되면 코드가 터지기 때문에 안전성 때문에 사용
					Timer::Get()->Print();   //FPS 출력
				if (Mouse::Get() != nullptr)
					Mouse::Get()->Print();   //마우스 좌표 출력
				if (program != nullptr)
					program->Render();
			}
			BitBlt(hdc, 0, 0, crtRect.right, crtRect.bottom, memDC, 0, 0, SRCCOPY);
			//이미지를 화면에 출력
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
		//지정된 창의 업데이트 영역에 사각형을 추가
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