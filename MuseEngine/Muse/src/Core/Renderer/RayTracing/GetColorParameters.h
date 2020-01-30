#pragma once

#include <glm/glm.hpp>

struct GetColorParameters
{
    glm::vec3 RayDirection;
	int Bounces = 0;
};

