#pragma once

#include "stdafx.h"

using namespace std;

struct WinDesc
{
	wstring AppName;
	HINSTANCE instance;
	HWND handle;
	UINT width;
	UINT height;
};

class Window
{
public:
	Window(WinDesc desc);
	~Window();

	WPARAM Run();

private:
	static LRESULT CALLBACK WndProc // 메세지 처리 함수
	(
		HWND handle,	// 어떤 창인지 알아보는 창의 식별자
		UINT message,	// 어떤 메세지가 들어왔는지 체크
		WPARAM wParam,	// 키보드의 관련된 정보 (어떤키가 눌렸는지)
		LPARAM lParam	// 마우스의 좌표, 클릭여부
	);

	static WinDesc desc;
	static class Program* program;
};