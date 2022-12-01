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
	static float timeElapsed; //이전 프레임에서 현재 프레임 시간의 사이

	int64 tickPerSecond;	 //초당 틱카운트
	int64 currentTime;		 //현재시간
	int64 lastTime;			 //마지막으로 기록된 시간
	int64 lastFPSUpdate;	 //마지막 FPS 업데이트 시간
	int64 fpsUpdateInterval; //FPS 업데이트 간격

	UINT frameCount;		 //초당 프레임 갯수
	float runningTime;		 //게임 시작부터 경과시간
	float framePerSecond;	 //FPS

	RECT fpsRect;			 //FPS 정보가 들어갈 상자
	wstring fpsStr;			 //FPS 정보가 들어갈 문자
};