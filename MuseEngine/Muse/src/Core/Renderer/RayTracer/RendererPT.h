#pragma once

#include <glm/glm.hpp>

namespace Muse
{
    struct GetColorParameters;

    class RendererPT
    {
    public:
        static glm::vec3 CalculateColor(GetColorParameters& a_GetColorParameters);
        static glm::vec3 CalculateCheckerBoardColor(const GetColorParameters& a_GetColorParameters);

    };
}
