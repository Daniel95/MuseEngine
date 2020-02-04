#pragma once

#include <glm/glm.hpp>

namespace Muse
{
    struct RayHitData;
    class RenderComponent;

    struct Ray
    {
        glm::vec3 Origin;
        glm::vec3 Direction;

        bool Cast() const;
        bool Cast(std::shared_ptr<const RenderComponent> a_Ignore, float a_MaxDistance = INFINITY) const;
        bool Cast(RayHitData& rayHitData, std::shared_ptr<const RenderComponent> a_Ignore = nullptr, float a_MaxDistance = INFINITY) const;
        /*
        bool Cast(std::vector<RayHitData>& a_RayHitData, float a_MaxDistance = INFINITY);
        bool Cast(std::vector<RayHitData>& a_RayHitDatas, const std::vector<std::shared_ptr<RenderComponent>>& a_RenderComponents, float a_MaxDistance = INFINITY);
        */

    };
}
