#pragma once

typedef INT64 int64;

class Timer :public SingletonBase<Timer>
{
public:
	friend class SingletonBase <Timer>;

	static bool Stopped() { return isTimerStopped; }
	static float Delta() { return isTimerStopped ? 0.0f : timeElapsed; }

	void Update();
	void Print();

	void Start();
	void Stop();

	float FPS() const { return framePerSecond; }
	float Running() const { return runningTime; }

private:
	Timer(void);
	~Timer(void);

	static bool isTimerStopped;
	static float timeElapsed; //���� �����ӿ��� ���� ������ �ð��� ����

	int64 tickPerSecond;	 //�ʴ� ƽī��Ʈ
	int64 currentTime;		 //����ð�
	int64 lastTime;			 //���������� ��ϵ� �ð�
	int64 lastFPSUpdate;	 //������ FPS ������Ʈ �ð�
	int64 fpsUpdateInterval; //FPS ������Ʈ ����

	UINT frameCount;		 //�ʴ� ������ ����
	float runningTime;		 //���� ���ۺ��� ����ð�
	float framePerSecond;	 //FPS

	RECT fpsRect;			 //FPS ������ �� ����
	wstring fpsStr;			 //FPS ������ �� ����
};