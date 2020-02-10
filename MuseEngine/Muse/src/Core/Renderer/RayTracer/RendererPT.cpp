#include "MusePCH.h"
#include "RendererPT.h"

#include "Core/Renderer/Material.h"
#include "GetColorParameters.h"

namespace Muse
{
    glm::vec3 RendererPT::CalculateColor(GetColorParameters& a_GetColorParameters)
    {
        const Material* material = a_GetColorParameters.Material;
        const MaterialType materialType = material->MaterialType;

        glm::vec3 color;

        switch (materialType)
        {
            case MaterialType::Checkerboard:
                color = CalculateCheckerBoardColor(a_GetColorParameters);
                break;
            default:
                color = material->Color;
        }

        return color;
    }

    glm::vec3 RendererPT::CalculateCheckerBoardColor(const GetColorParameters& a_GetColorParameters)
    {
        int gridPositionX = static_cast<int>(std::round(a_GetColorParameters.IntersectionPoint.x / a_GetColorParameters.Material->GridSize));
        int gridPositionZ = static_cast<int>(std::round(a_GetColorParameters.IntersectionPoint.z / a_GetColorParameters.Material->GridSize));

        int positionInGrid = gridPositionX + gridPositionZ;

        glm::vec3 result = a_GetColorParameters.Material->SecondaryColor;

        if (positionInGrid % 2 == 0)
        {
            result = a_GetColorParameters.Material->Color;
        }

        return result;
    }
}
