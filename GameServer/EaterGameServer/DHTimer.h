#pragma once
#include <chrono>

class DHTimer
{
private:
	unsigned int m_FPS = 60;
	double m_Frame_To_Second = 0.f;
		
	std::chrono::time_point<std::chrono::system_clock> Prev_Time;							// ���� ������ �ð�
	std::chrono::time_point<std::chrono::system_clock> Current_Time;						// ���� ������ �ð�
	std::chrono::duration<double> Passed_Time = std::chrono::duration<double>::zero();		// �����ð�
public:
	DHTimer();
	~DHTimer();

	void SetFrame(unsigned int _FPS);
	// true �϶� �����ϸ��.
	bool Ready_Frame();
};

