#pragma once

#include <glm/glm.hpp>
#include "Core/Gameplay/Component/RenderComponent.h"
#include "RayHitData.h"

namespace Muse
{
    class Shape;
    struct Material;

    struct GetColorParameters
    {
        std::shared_ptr<const RenderComponent> RenderComponent;
        Ray* Ray;
        glm::vec3 IntersectionPoint;
        int Bounces = 0;

        const Material& GetMaterial();
        std::shared_ptr<const Shape> GetShape();
    };
}
