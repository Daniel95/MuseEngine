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
        bool Cast(std::shared_ptr<const RenderComponent> a_Ignore, float a_MaxDistance = INFINITY) const;
        bool Cast(RayHitData& rayHitData, std::shared_ptr<const RenderComponent> a_Ignore = nullptr, float a_MaxDistance = INFINITY) const;
    };
}
