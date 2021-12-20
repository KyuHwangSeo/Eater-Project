#pragma once
#include <random>

class RandomBase
{
protected:
	static std::default_random_engine g_RandomEngine;
};

std::default_random_engine RandomBase::g_RandomEngine;

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

void RandomNumber<float>::SetRange(float min, float max)
{
	m_RandomGenerator = std::uniform_real_distribution<float>(min, max);
}

void RandomNumber<float>::Reset()
{
	m_RandomGenerator.reset();
}

float RandomNumber<float>::GetRandomNumber()
{
	return m_RandomGenerator(g_RandomEngine);
}

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

void RandomNumber<int>::SetRange(int min, int max)
{
	m_RandomGenerator = std::uniform_int_distribution<int>(min, max);
}

void RandomNumber<int>::Reset()
{
	m_RandomGenerator.reset();
}

int RandomNumber<int>::GetRandomNumber()
{
	return m_RandomGenerator(g_RandomEngine);
}
