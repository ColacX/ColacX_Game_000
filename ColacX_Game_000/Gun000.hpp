//plasma bullet gun effect

#pragma once

#include "Camera000.h"

class Gun000
{
private:
	glm::vec3 position;
	glm::mat4 rotation;

	glm::vec3 target;
	float sprayRadius;
	float cooldown;

public:
	void Construct()
	{
	}

	void G_Construct()
	{
	}

	void Run()
	{
	}

	void G_Draw(Camera000& camera000, unsigned int framecount)
	{

	}
};