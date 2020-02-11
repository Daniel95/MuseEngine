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
        std::shared_ptr<const Shape> Shape;
        const Material* Material;
        Ray* Ray;
        glm::vec3 IntersectionPoint;
        int Bounces = 0;

        //const std::shared_ptr<const RenderComponent> RenderComponent;
    };

}
