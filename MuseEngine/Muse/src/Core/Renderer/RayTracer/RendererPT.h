#pragma once

#include <glm/glm.hpp>

namespace Muse
{
    struct Material;

    class RendererPT
    {
    public:
        static glm::vec3 CalculateColor(const Material& a_Material, const glm::vec3& a_IntersectionPoint, bool& a_ContinueSampling);
        static glm::vec3 CalculateCheckerBoardColor(const Material& a_Material, const glm::vec3& a_IntersectionPoint);

    };
}
