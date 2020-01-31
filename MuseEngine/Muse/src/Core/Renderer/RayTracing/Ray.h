#pragma once

#include <glm/glm.hpp>

namespace Muse
{
    struct Ray
    {
        glm::vec3 Origin;
        glm::vec3 Direction;
    };
}