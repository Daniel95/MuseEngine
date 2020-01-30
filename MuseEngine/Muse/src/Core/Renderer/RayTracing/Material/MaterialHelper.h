#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace Muse
{
    class GameObject;
    class LightSource;

    glm::vec3 operator*(const glm::vec3& a_Lhs, float a_Rhs);
    void FilterBlockedLights(std::vector<LightSource*>& lights, const GameObject& sceneObject, const glm::vec3& point);
}
