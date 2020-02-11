#include "MusePCH.h"
#include "RendererPT.h"

#include "Core/Renderer/Material.h"
#include "GetColorParameters.h"

namespace Muse
{
    glm::vec3 RendererPT::CalculateColor(const Material& a_Material, const glm::vec3& a_IntersectionPoint, bool& a_ContinueSampling)
    {
        const MaterialType materialType = a_Material.MaterialType;

        glm::vec3 color;

        switch (materialType)
        {
            case MaterialType::Checkerboard:
            {
                    
                color = CalculateCheckerBoardColor(a_Material, a_IntersectionPoint);
                a_ContinueSampling = true;
                break;
            }
            case MaterialType::Light:
            {
                    
                color = a_Material.Color;
                a_ContinueSampling = false;
                break;

            }
            default:
            {
                    
                color = a_Material.Color;
                a_ContinueSampling = true;
            }
        }

        return color;
    }

    glm::vec3 RendererPT::CalculateCheckerBoardColor(const Material& a_Material, const glm::vec3& a_IntersectionPoint)
    {
        int gridPositionX = static_cast<int>(std::round(a_IntersectionPoint.x / a_Material.GridSize));
        int gridPositionZ = static_cast<int>(std::round(a_IntersectionPoint.z / a_Material.GridSize));

        int positionInGrid = gridPositionX + gridPositionZ;

        glm::vec3 result = a_Material.SecondaryColor;

        if (positionInGrid % 2 == 0)
        {
            result = a_Material.Color;
        }

        return result;
    }
}
