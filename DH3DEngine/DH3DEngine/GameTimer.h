#pragma once

class GameTimer
{
public:
	GameTimer();

	float TotalTime() const;	// 초 단위로 반환.
	float DeltaTime() const;	// 초 단위로 반환.

	void Reset();	// 메세지 루프가 호출 되기 전에 호출해야 한다.
	void Start();	// 시작 될때 호출
	void Stop();	// 멈출 때 호출.
	void Tick();	// 매 프레임 마다 호출.

private:
	double mSecondsPerCount = 0.0f;	// 시간으로 변경해주기 위한 변수 (초당 횟수의 역수)
	double mDeltaTime = -1.0f;		// 이전 프레임과의 시간 간격.

	__int64 mBaseTime = 0;			// 일시정지 시 멈춘 시간을 제외하고 총 시간을 반환해주기 위해서 생성. (타이머가 생성된 그 시점의 시간을 저장)
	__int64 mPausedTime = 0;		// 일시정지 된 시간.
	__int64 mStopTime = 0;			// 멈추기 시작한 시간.
	__int64 mPrevTime = 0;			// 이전 프레임의 시간을 저장.
	__int64 mCurrTime = 0;			// 현재 프레임을 저장해두기 위한 변수.

	bool mStopped = false;			// 현재 일시정지가 되어있는 
};

