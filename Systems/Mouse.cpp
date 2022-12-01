#include "stdafx.h"
#include "Mouse.h"

Mouse::Mouse()
{
	position = Vector3(0, 0, 0);

	wheelStatus    = Vector3(0.0f, 0.0f, 0.0f);
	wheelOldStatus = Vector3(0.0f, 0.0f, 0.0f);
	wheelMoveValue = Vector3(0.0f, 0.0f, 0.0f);

	ZeroMemory(buttonStatus   , sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonOldStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonMap      , sizeof(byte) * MAX_INPUT_MOUSE);

	ZeroMemory(startDblClk, sizeof(DWORD) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonCount, sizeof(int  ) * MAX_INPUT_MOUSE);

	timeDblClk = GetDoubleClickTime(); //마우스의 현재 두 번 클릭 시간을 검색
	startDblClk[0] = GetTickCount(); //OS 부팅할 때부터 지나간 시간을 msec 단위로 올려줌 (1초 = 1000)
	for (int i = 1; i < MAX_INPUT_MOUSE; i++)
		startDblClk[i] = startDblClk[0];

	DWORD tLine = 0; //내려가는 줄 수 (기본값 : 3)
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &tLine, 0);
	//파라미터를 통해 윈도우즈의 시스템 설정을 변경하거나 현재 시스템의 설정 값을 얻어오는 변수
	//수직 스크롤

	PosRect = { WinMaxWidth - 200, 0, WinMaxHeight, 25 };
	PosStr = L"";
}

Mouse::~Mouse() {}

void Mouse::Update()
{
	memcpy(buttonOldStatus, buttonStatus, sizeof(buttonOldStatus));

	ZeroMemory(buttonStatus, sizeof(buttonStatus));
	ZeroMemory(buttonMap   , sizeof(buttonMap));

	/*
	GetAsyncKeyState
	키가 눌러져 있으면 최상위 비트를 1로 만들고 CapsLock, ScrollLock 같은 토글키가 ON 되어있으면 최하위 비트를 1로 만든다
	 -> 특정키의 눌러짐 상태를 조사하고 싶다 = & 0x8000
	*/
	buttonStatus[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;
	//키가 눌러져 있으면 최상위 비트를 1로 만들고 CapsLock, ScrollLock 같은 토글키가 ON 되어 있으면 최하위 비트를 1로 만듦
	//특정키의 눌러짐 상태를 조사하고 싶으면  == 0x8000

	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		int tOldStatus = buttonOldStatus[i];
		int tStatus    = buttonStatus[i];

		if      (tOldStatus == 0 && tStatus == 1)
			buttonMap[i] = BUTTON_INPUT_STATUS_DOWN;
		else if (tOldStatus == 1 && tStatus == 0)
			buttonMap[i] = BUTTON_INPUT_STATUS_UP;
		else if (tOldStatus == 1 && tStatus == 1)
			buttonMap[i] = BUTTON_INPUT_STATUS_PRESS;
		else
			buttonMap[i] = BUTTON_INPUT_STATUS_NONE;
	}

	POINT point;
	GetCursorPos(&point);			//전체화면 기준의 마우스 좌표를 얻어옴
	ScreenToClient(handle, &point); //전체화면 기준의 마우스 좌표를 윈도우 클라이언트 좌표로 리턴

	wheelOldStatus.x = wheelStatus.x;
	wheelOldStatus.y = wheelStatus.y;

	wheelStatus.x = point.x;
	wheelStatus.y = point.y;

	wheelMoveValue = wheelStatus - wheelOldStatus;

	DWORD tButtonStatus = GetTickCount();
	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		if (buttonCount[i] == BUTTON_INPUT_STATUS_DOWN)
		{
			if (buttonCount[i] == 1)
			{
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk)
					buttonCount[i] = 0;
			}
		}
		buttonCount[i]++;

		if (buttonCount[i] == 1)
			startDblClk[i] = tButtonStatus;


		if (buttonMap[i] == BUTTON_INPUT_STATUS_UP)
		{
			if (buttonCount[i] == 1)
			{
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk)
					buttonCount[i] = 0;
			}
			else if (buttonCount[i] == 2)
			{
				if ((tButtonStatus - startDblClk[i]) <= timeDblClk)
					buttonCount[i] = BUTTON_INPUT_STATUS_DBLCLK;

				buttonCount[i] = 0;
			}
		}
	}
}

void Mouse::Print()
{
	PosStr = L"X : " + to_wstring((int)position.x) + L", Y : " + to_wstring((int)position.y);
	DrawText(memDC, PosStr.c_str(), -1, &PosRect, DT_CENTER | DT_VCENTER);
	//텍스트를 특정 사각형 안에 출력
	//매개변수는 핸들, 문자열, 길이(-1이면 널 종료 문자열까자 포함해서 자동으로 계산), 텍스트를 출력할 RECT 구조체의 사각형, 텍스트 서식
}

LRESULT Mouse::InputProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN || message == WM_MOUSEMOVE)
	{
		position.x = LOWORD(lParam); //x값
		position.y = HIWORD(lParam); //y값
	}

	if (message == WM_MOUSEWHEEL)
	{
		short tWheelValue = (short)HIWORD(wParam);

		wheelOldStatus.z = wheelStatus.z;
		wheelStatus.z += (float)tWheelValue;
	}
	return TRUE; //대소문자 상관없음
}