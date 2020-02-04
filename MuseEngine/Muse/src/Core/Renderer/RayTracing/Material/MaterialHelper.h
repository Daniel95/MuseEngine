#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace Muse
{
    class RenderComponent;
    class LightSource;

    glm::vec3 operator*(const glm::vec3& a_Lhs, float a_Rhs);
}
