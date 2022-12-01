#include "stdafx.h"
#include "Timer.h"

bool Timer::isTimerStopped = true;
float Timer::timeElapsed = 0.0f;

Timer::Timer(void)
	:tickPerSecond(0), currentTime(0), lastTime(0), lastFPSUpdate(0),
	fpsUpdateInterval(0), frameCount(0), runningTime(0), framePerSecond(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)& tickPerSecond);
	//���ػ� Ÿ�̸��� �ֱ�(1�ʴ� ���� ��(Hz))�� ��ȯ
	//IpFrequency���� �ش� Ÿ�̸��� �ֱⰡ ������ (���⼭�� tickPerSecond)
	fpsUpdateInterval = tickPerSecond >> 1;
	//1�ʿ� �´� ������ 1��
	//�������� �ʹ� Ŀ�� 1�ʿ� �°� ����

	fpsRect = { WinMaxWidth - 175, 25, WinMaxHeight, 50 };
	fpsStr = L"";
}

Timer::~Timer(void) {}

void Timer::Update()
{
	if (isTimerStopped) return;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	//�ð� ���� ������ ����� �� �ִ� ���ػ� Ÿ�� �������� ���� ī������ ���� ���� �˻�
	//���� CPU�� ƽ�� �޾ƿ��� ��
	//���� ���� ī���� ���� ����� �޴� ������ ���� �����͸� �Ѱ���
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
	//������ �߻��� �� ���� ���� �־�θ� ������ ���α׷��� �����ڿ��� �˷���
	//�ش� �Լ��� �ɸ��� ���� �߻� ��ġ, call, stack�� ���� ������ �˰� ��

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