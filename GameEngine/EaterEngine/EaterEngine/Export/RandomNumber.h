#pragma once
#include <random>

class RandomBase
{
protected:
	static std::default_random_engine g_RandomEngine;
};

template<typename T>
class RandomNumber {};

template<>
class RandomNumber<float> : public RandomBase
{
public:
	// �ش� ���� ���� �ʱ�ȭ..
	void SetRange(float min, float max);

	// �ش� ���� ĳ�� �ʱ�ȭ..
	void Reset();

	// �ش� ���� ���� ���� ����..
	float GetRandomNumber();

private:
	std::uniform_real_distribution<float> m_RandomGenerator;
};

template<>
class RandomNumber<int> : public RandomBase
{
public:
	// �ش� ���� ���� �ʱ�ȭ..
	void SetRange(int min, int max);

	// �ش� ���� ĳ�� �ʱ�ȭ..
	void Reset();

	// �ش� ���� ���� ���� ����..
	int GetRandomNumber();

private:
	std::uniform_int_distribution<int> m_RandomGenerator;
};