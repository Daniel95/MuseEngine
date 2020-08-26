#include "MusePCH.h"
#include "RendererRT.h"

#include "Core/Renderer/Material.h"
#include "Core/Renderer/RayTracer/GetColorParameters.h"
#include "Core/Renderer/RayTracer/LightSource.h"
#include "Core/Scene/SceneManagerOld.h"
#include "Core/Renderer/RayTracer/Shape/Shape.h"
#include "Core/Renderer/RayTracer/Ray.h"
#include "Core/Scene/SceneOld.h"

namespace Muse
{
    glm::vec3 RendererRT::CalculateColor(GetColorParameters& a_GetColorParameters)
    {
        if (a_GetColorParameters.Bounces <= 0)
        {
            return SceneManagerOld::GetActiveScene()->GetBackgroundColor();
        }

        const Material& material = a_GetColorParameters.GetMaterial();
        const MaterialType materialType = a_GetColorParameters.GetMaterial().MaterialType;

        glm::vec3 color;

        switch (materialType) {
            case MaterialType::Diffuse:
                color = CalculateDiffuseColor(a_GetColorParameters);
                break;
            case MaterialType::Specular:
                color = CalculateSpecularColor(a_GetColorParameters);
                break;
            case MaterialType::BlinnPhong:
                color = CalculateBlinnPhongColor(a_GetColorParameters);
                break;
            case MaterialType::Reflective:
                color = CalculateReflectiveColor(a_GetColorParameters);
                break;
            case MaterialType::Refractive:
                color = CalculateRefractiveColor(a_GetColorParameters);
                break;
            case MaterialType::Checkerboard:
                color = CalculateCheckerBoardColor(a_GetColorParameters);
                break;
            case MaterialType::NormalDebug:
                color = CalculateDebugNormalsColor(a_GetColorParameters);
                break;
            default:
                color = material.Color;
                //LOG_ENGINE_ERROR("MaterialType: {0} not supported", static_cast<int>(materialType));
                //ASSERT_ENGINE(false, "Material type not supported!");
        }

        return color;
    }

    float RendererRT::GetDiffuse(const glm::vec3& a_Normal, const glm::vec3& a_DirectionToLightSource)
    {
        const float diffuseStrength = glm::dot(a_DirectionToLightSource, a_Normal);
        const float clampedDiffuseStrength = std::clamp(diffuseStrength, 0.0f, 1.0f);

        return clampedDiffuseStrength;
    }

    float RendererRT::GetSpecular(const glm::vec3& a_Normal, const glm::vec3& a_LookDirection,
        const glm::vec3& a_DirectionToLightSource, float a_Strength)
    {
        const glm::vec3 reflectionDirection = glm::reflect(a_DirectionToLightSource, a_Normal);
        const float specularValue = std::pow(std::max(glm::dot(a_LookDirection, reflectionDirection), 0.0f), 32.0f) * a_Strength;
        const float clampedspeculairStrength = std::clamp(specularValue, 0.0f, 1.0f);

        return clampedspeculairStrength;
    }

    glm::vec3 RendererRT::CalculateDiffuseColor(GetColorParameters& a_GetColorParameters)
    {
        const std::vector<std::shared_ptr<LightSource>>& lightSources = SceneManagerOld::GetActiveScene()->GetLightSources();

        glm::vec3 totalDiffuse = glm::vec3(0);
        glm::vec3 normal = a_GetColorParameters.GetShape()->GetNormal(a_GetColorParameters.IntersectionPoint);

        for (const std::shared_ptr<LightSource>& lightSource : lightSources)
        {
            const glm::vec3 lightPosition = lightSource->GetPosition();
            const glm::vec3 directionToLightSource = glm::normalize(lightPosition - a_GetColorParameters.IntersectionPoint);
            Ray ray{ a_GetColorParameters.IntersectionPoint, directionToLightSource };

            if (!ray.Cast())
            {
                const glm::vec3 directionToLightSource = glm::normalize(lightPosition - a_GetColorParameters.IntersectionPoint);
                const float diffuseStrength = glm::dot(directionToLightSource, normal);
                const float clampedDiffuseStrength = std::clamp(diffuseStrength, 0.0f, 1.0f);
                const glm::vec3 lightColor = lightSource->GetLight(a_GetColorParameters.IntersectionPoint) * clampedDiffuseStrength;

                totalDiffuse += lightColor;
            }
        }

        return totalDiffuse;
    }

    glm::vec3 RendererRT::CalculateSpecularColor(GetColorParameters& a_GetColorParameters)
    {
        const std::vector<std::shared_ptr<LightSource>>& lightSources = SceneManagerOld::GetActiveScene()->GetLightSources();

        glm::vec3 totalSpecular = glm::vec3(0);
        glm::vec3 normal = a_GetColorParameters.GetShape()->GetNormal(a_GetColorParameters.IntersectionPoint);

        for (const std::shared_ptr<LightSource>& lightSource : lightSources)
        {
            const glm::vec3 lightPosition = lightSource->GetPosition();
            const glm::vec3 directionToLightSource = glm::normalize(lightPosition - a_GetColorParameters.IntersectionPoint);
            Ray ray{ a_GetColorParameters.IntersectionPoint, directionToLightSource };

            if (!ray.Cast())
            {
                float specular = GetSpecular(normal, a_GetColorParameters.Ray->Direction, directionToLightSource, a_GetColorParameters.GetMaterial().SpecularStrength);

                totalSpecular += specular;
            }
        }

        return totalSpecular;
    }

    glm::vec3 RendererRT::CalculateBlinnPhongColor(GetColorParameters& a_GetColorParameters)
    {
        glm::vec3 specularAndDiffuse = glm::vec3(0);
        glm::vec3 normal = a_GetColorParameters.GetShape()->GetNormal(a_GetColorParameters.IntersectionPoint);

        const std::vector<std::shared_ptr<LightSource>>& lightSources = SceneManagerOld::GetActiveScene()->GetLightSources();

        Ray ray{ a_GetColorParameters.IntersectionPoint };

        for (const std::shared_ptr<LightSource>& lightSource : lightSources)
        {
            const glm::vec3 lightPosition = lightSource->GetPosition();
            const glm::vec3 directionToLightSource = glm::normalize(lightPosition - a_GetColorParameters.IntersectionPoint);
            glm::vec3 light = lightSource->GetLight(a_GetColorParameters.IntersectionPoint);

            ray.Direction = directionToLightSource;

            if (!ray.Cast())
            {
                float specular = GetSpecular(normal, a_GetColorParameters.Ray->Direction, directionToLightSource, a_GetColorParameters.GetMaterial().SpecularStrength);
                float diffuse = GetDiffuse(normal, directionToLightSource);

                specularAndDiffuse += (specular + diffuse) * light;
            }
        }

        const glm::vec3 combinedLights = specularAndDiffuse + SceneManagerOld::GetActiveScene()->GetAmbientLight();

        return combinedLights;
    }

    glm::vec3 RendererRT::CalculateReflectiveColor(GetColorParameters& a_GetColorParameters)
    {
        glm::vec3 reflectionColor = SceneManagerOld::GetActiveScene()->GetBackgroundColor();

        if (a_GetColorParameters.Bounces <= 0)
        {
            return reflectionColor;
        }
        a_GetColorParameters.Bounces--;

        const glm::vec3 normal = a_GetColorParameters.GetShape()->GetNormal(a_GetColorParameters.IntersectionPoint);

        const glm::vec3 reflectionDirection = glm::reflect(a_GetColorParameters.Ray->Direction, normal);

        RayHitData rayHitData;
        a_GetColorParameters.Ray->Origin = a_GetColorParameters.IntersectionPoint; 
        a_GetColorParameters.Ray->Direction = reflectionDirection;

        if (a_GetColorParameters.Ray->Cast(rayHitData, a_GetColorParameters.RenderComponent))
        {
            a_GetColorParameters.IntersectionPoint = rayHitData.UpdateIntersectionPoint(*a_GetColorParameters.Ray);
            a_GetColorParameters.RenderComponent = rayHitData.m_RenderComponent;

            reflectionColor = CalculateColor(a_GetColorParameters);
        }

        return reflectionColor;
    }

    glm::vec3 RendererRT::CalculateRefractiveColor(GetColorParameters& a_GetColorParameters)
    {
        glm::vec3 refractionColor = SceneManagerOld::GetActiveScene()->GetBackgroundColor();

        if (a_GetColorParameters.Bounces <= 0)
        {
            return refractionColor;
        }
        a_GetColorParameters.Bounces--;

        const glm::vec3 normal = a_GetColorParameters.GetShape()->GetNormal(a_GetColorParameters.IntersectionPoint);

        glm::vec3 refractionDirection = glm::refract(a_GetColorParameters.Ray->Direction, normal, a_GetColorParameters.GetMaterial().Eta);

        RayHitData rayHitData;
        a_GetColorParameters.Ray->Origin = a_GetColorParameters.IntersectionPoint;
        a_GetColorParameters.Ray->Direction = refractionDirection;

        if (a_GetColorParameters.Ray->Cast(rayHitData, a_GetColorParameters.RenderComponent))
        {
            a_GetColorParameters.IntersectionPoint = rayHitData.UpdateIntersectionPoint(*a_GetColorParameters.Ray);
            a_GetColorParameters.RenderComponent = rayHitData.m_RenderComponent;

            refractionColor = CalculateColor(a_GetColorParameters);
        }

        return refractionColor;
    }

    glm::vec3 RendererRT::CalculateCheckerBoardColor(GetColorParameters& a_GetColorParameters)
    {
        const glm::vec3 light = CalculateBlinnPhongColor(a_GetColorParameters);

        int gridPositionX = static_cast<int>(std::round(a_GetColorParameters.IntersectionPoint.x / a_GetColorParameters.GetMaterial().GridSize));
        int gridPositionZ = static_cast<int>(std::round(a_GetColorParameters.IntersectionPoint.z / a_GetColorParameters.GetMaterial().GridSize));

        int positionInGrid = gridPositionX + gridPositionZ;

        glm::vec3 result = a_GetColorParameters.GetMaterial().SecondaryColor;

        if (positionInGrid % 2 == 0)
        {
            result = a_GetColorParameters.GetMaterial().Color;
        }

        return result * light;
    }

    glm::vec3 RendererRT::CalculateDebugNormalsColor(GetColorParameters& a_GetColorParameters)
    {
        a_GetColorParameters;

        const glm::vec3 normalT = a_GetColorParameters.GetShape()->GetNormal(a_GetColorParameters.IntersectionPoint);

        const glm::vec3 normal = (normalT + glm::vec3(1, 1, 1)) / 2.f;
        const glm::vec3 normalDebugColor = glm::vec3(normal.x * a_GetColorParameters.GetMaterial().Color.r, normal.y * a_GetColorParameters.GetMaterial().Color.g, normal.z * a_GetColorParameters.GetMaterial().Color.b);
        return normalDebugColor;
    }
}
