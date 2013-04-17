#pragma once

class Random
{
private:
public:
	void Seed(unsigned int seed);
	float Next();
	float Next(float a, float b);
};