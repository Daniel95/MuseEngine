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
        std::shared_ptr<RenderComponent> RenderComponent;
        const Material* Material;
        std::shared_ptr<Shape> Shape;
        Ray* Ray;
        glm::vec3 IntersectionPoint;
        int Bounces = 0;
    };

}
