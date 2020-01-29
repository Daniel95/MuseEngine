#pragma once

#include "MathLibrary.h"
#include "vec3.h"

struct GetColorParameters
{
	vec3 RayDirection;
	int Bounces = 0;
};

