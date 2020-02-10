#pragma once

#include <glm/glm.hpp>

namespace Muse
{
    class Color
    {
        static glm::vec3 GetRed() { return glm::vec3(1, 0, 0); }
        static glm::vec3 GetGreen() { return glm::vec3(0, 1, 0); }
        static glm::vec3 GetBlue() { return glm::vec3(0, 0, 1); }
        static glm::vec3 GetPink() { return glm::vec3(1, 0, 1); }
    };
}
