#include "stdafx.h"
#include "Timer.h"

bool Timer::isTimerStopped = true;
float Timer::timeElapsed = 0.0f;

Timer::Timer(void)
	:tickPerSecond(0), currentTime(0), lastTime(0), lastFPSUpdate(0),
	fpsUpdateInterval(0), frameCount(0), runningTime(0), framePerSecond(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)& tickPerSecond);
	//고해상도 타이머의 주기(1초당 진동 수(Hz))를 반환
	//IpFrequency에는 해당 타이머의 주기가 설정됨 (여기서는 tickPerSecond)
	fpsUpdateInterval = tickPerSecond >> 1;
	//1초에 맞는 진동수 1초
	//진동수가 너무 커서 1초에 맞게 조절

	fpsRect = { WinMaxWidth - 175, 25, WinMaxHeight, 50 };
	fpsStr = L"";
}

Timer::~Timer(void) {}

void Timer::Update()
{
	if (isTimerStopped) return;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	//시간 간격 측정에 상용할 수 있는 고해상도 타임 스탬프인 성능 카운터의 현재 값을 검색
	//현재 CPU의 틱을 받아오는 것
	//현재 성능 카운터 값을 계수로 받는 변수에 대한 포인터를 넘겨줌
	timeElapsed = (float)(currentTime - lastTime) / (float)tickPerSecond;
	runningTime += timeElapsed;

	frameCount++;
	if (currentTime - lastFPSUpdate >= fpsUpdateInterval)
	{
		float tempCurrentTime = (float)currentTime / (float)tickPerSecond;
		float tempLastTime = (float)lastFPSUpdate / (float)tickPerSecond;
		framePerSecond = (float)frameCount / (tempCurrentTime - tempLastTime);

		lastFPSUpdate = (INT64)currentTime;
		frameCount = 0;
	}

	lastTime = currentTime;
}

void Timer::Print()
{
	fpsStr = L"FPS : " + to_wstring((int)FPS());
	DrawText(memDC, fpsStr.c_str(), -1, &fpsRect, DT_CENTER | DT_VCENTER);
}

void Timer::Start()
{
	if (!isTimerStopped)
		assert(false);
	//오류가 발생할 것 같은 곳에 넣어두면 저절로 프로그램이 개발자에게 알려줌
	//해당 함수에 걸리면 버그 발생 위치, call, stack등 여러 정보를 알게 됨

	QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
	isTimerStopped = false;
}

void Timer::Stop()
{
	if (isTimerStopped)
		assert(false);

	INT64 stopTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);
	runningTime += (float)(stopTime - lastTime) / (float)tickPerSecond;
	isTimerStopped = true;
}