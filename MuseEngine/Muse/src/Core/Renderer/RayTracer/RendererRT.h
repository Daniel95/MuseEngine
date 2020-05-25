#pragma once

#include <glm/glm.hpp>

namespace Muse
{
    struct GetColorParameters;

    class RendererRT
    {
    public:
        static glm::vec3 CalculateColor(GetColorParameters& a_GetColorParameters);

    private:

        static float GetDiffuse(const glm::vec3& a_Normal, const glm::vec3& a_DirectionToLightSource);
        static float GetSpecular(const glm::vec3& a_Normal, const glm::vec3& a_LookDirection, const glm::vec3& a_DirectionToLightSource, float a_Strength);
        //static glm::vec3 GetRefraction( , const glm::vec3& a_Point, GetColorParameters& a_GetColorParameters)

        static glm::vec3 CalculateDiffuseColor(GetColorParameters& a_GetColorParameters);
        static glm::vec3 CalculateSpecularColor(GetColorParameters& a_GetColorParameters);
        static glm::vec3 CalculateBlinnPhongColor(GetColorParameters& a_GetColorParameters);
        static glm::vec3 CalculateReflectiveColor(GetColorParameters& a_GetColorParameters);
        static glm::vec3 CalculateRefractiveColor(GetColorParameters& a_GetColorParameters);
        static glm::vec3 CalculateCheckerBoardColor(GetColorParameters& a_GetColorParameters);
        static glm::vec3 CalculateDebugNormalsColor(GetColorParameters& a_GetColorParameters);


    };
}
