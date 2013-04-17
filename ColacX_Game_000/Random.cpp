#include "Random.h"

#include <stdlib.h>
#include <time.h>

void Random::Seed(unsigned int seed)
{
	srand(seed);
}

float Random::Next()
{
	return (float)rand() / (float)RAND_MAX;
}

float Random::Next(float a, float b)
{
	return a + (b - a) * Next();
}