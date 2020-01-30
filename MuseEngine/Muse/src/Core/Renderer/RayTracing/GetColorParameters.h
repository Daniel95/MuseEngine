#pragma once

#include <glm/glm.hpp>


namespace Muse
{

    struct GetColorParameters
    {
        glm::vec3 RayDirection;
        int Bounces = 0;
    };

}