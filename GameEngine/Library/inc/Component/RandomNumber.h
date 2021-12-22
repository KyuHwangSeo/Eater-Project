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
	// 해당 난수 범위 초기화..
	void SetRange(float min, float max);

	// 해당 난수 캐시 초기화..
	void Reset();

	// 해당 범위 내의 난수 추출..
	float GetRandomNumber();

private:
	std::uniform_real_distribution<float> m_RandomGenerator;
};

template<>
class RandomNumber<int> : public RandomBase
{
public:
	// 해당 난수 범위 초기화..
	void SetRange(int min, int max);

	// 해당 난수 캐시 초기화..
	void Reset();

	// 해당 범위 내의 난수 추출..
	int GetRandomNumber();

private:
	std::uniform_int_distribution<int> m_RandomGenerator;
};