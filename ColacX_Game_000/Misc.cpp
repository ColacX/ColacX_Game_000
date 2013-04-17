#include "Misc.h"

#include <math.h>

Misc::Misc()
{
}

float Misc::Distance( float x0, float y0, float z0, float x1, float y1, float z1)
{
	float dx = x0 - x1;
	float dy = y0 - y1;
	float dz = z0 - z1;
	return sqrtf( dx*dx + dy*dy + dz*dz );
}