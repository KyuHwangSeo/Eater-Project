#include "RandomNumber.h"

std::default_random_engine RandomBase::g_RandomEngine;

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
