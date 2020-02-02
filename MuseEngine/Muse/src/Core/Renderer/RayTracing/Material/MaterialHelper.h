#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace Muse
{
    class RenderComponent;
    class LightSource;

    glm::vec3 operator*(const glm::vec3& a_Lhs, float a_Rhs);
    void FilterBlockedLights(std::vector<std::shared_ptr<LightSource>>& lights, std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point);
}
