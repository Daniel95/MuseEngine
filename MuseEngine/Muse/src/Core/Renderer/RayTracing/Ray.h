#pragma once

#include <glm/glm.hpp>

namespace Muse
{
    class RayHitData;
    class RenderComponent;

    struct Ray
    {
        glm::vec3 Origin;
        glm::vec3 Direction;

        bool Cast() const;
        bool Cast(const std::shared_ptr<const RenderComponent>& a_Ignore, float a_MaxDistance = INFINITY) const;
        bool Cast(RayHitData& rayHitData, const std::shared_ptr<const RenderComponent>& a_Ignore = nullptr, float a_MaxDistance = INFINITY) const;
    };
}
