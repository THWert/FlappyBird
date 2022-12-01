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
	static LRESULT CALLBACK WndProc // �޼��� ó�� �Լ�
	(
		HWND handle,	// � â���� �˾ƺ��� â�� �ĺ���
		UINT message,	// � �޼����� ���Դ��� üũ
		WPARAM wParam,	// Ű������ ���õ� ���� (�Ű�� ���ȴ���)
		LPARAM lParam	// ���콺�� ��ǥ, Ŭ������
	);

	static WinDesc desc;
	static class Program* program;
};