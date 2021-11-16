
#include "GameTimer.h"
#include <windows.h>

GameTimer::GameTimer()
{
	__int64 Counts_Per_Sec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&Counts_Per_Sec);
	mSecondsPerCount = 1.0 / (double)Counts_Per_Sec;
}

// Reset()이 호출 된 시점으로 부터의 경과 시간이 리턴 된다.
// 만약 일시정지 상태라고 하더라도 일시정지 시간이 제외된 총 경과 시간이 리턴 된다.
float GameTimer::TotalTime() const
{
	/// 만약 일시 정지 상태라면..
	if (mStopped)
	{
		return (float)(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
	/// 일시 정지 상태가 아니라면..
	else
	{
		return (float)(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
}

// DTime을 리턴해 준다.
float GameTimer::DeltaTime() const
{
	return (float)mDeltaTime;
}

void GameTimer::Reset()
{
	__int64 Current_Time;
	QueryPerformanceCounter((LARGE_INTEGER*)&Current_Time);
	
	// Reset()을 호출 한 시점을 BaseTime에 저장 해둔다.
	mBaseTime = Current_Time;
	mPrevTime = Current_Time;
	// 일시 정지를 풀어 줌.
	mStopTime = 0;
	mStopped = false;
}

void GameTimer::Start()
{
	__int64 Current_Time;
	QueryPerformanceCounter((LARGE_INTEGER*)&Current_Time);

	// 만약 정지 상태일때 Start()를 호출 하면 정지 상태를 해제 시켜준다.
	if (mStopped)
	{
		// 이전에 만약 정지 상태라면 일시 정지 된 시간은 현재 시작시간에서 정지 시간을 뺀 값이다.
		mPausedTime += (Current_Time - mStopTime);

		mPrevTime = Current_Time;
		mStopTime = 0;
		mStopped = false;
	}
}

void GameTimer::Stop()
{
	// 만약 정지상태가 아닐 때 Stop()을 호출 하게 된다면
	if (!mStopped)
	{
		__int64 Current_Time;
		QueryPerformanceCounter((LARGE_INTEGER*)&Current_Time);

		// 정지 를 시작한 시간을 현재 시간으로 저장해 두고 정지되었다는 플래그를 켠다.
		mStopTime = Current_Time;
		mStopped = true;
	}
}

void GameTimer::Tick()
{
	// 만약 정지상태 라면 DTime은 0으로 설정하고 리턴한다.
	if (mStopped)
	{
		mDeltaTime = 0.0;
		return;
	}

	__int64 Current_Time;
	QueryPerformanceCounter((LARGE_INTEGER*)&Current_Time);
	
	mCurrTime = Current_Time;
	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;
	mPrevTime = mCurrTime;

	// 만약 경과 프레임이 음수 일 경우를 대비하여..
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}
