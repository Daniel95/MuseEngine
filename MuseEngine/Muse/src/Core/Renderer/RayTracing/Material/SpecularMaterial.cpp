#include "MusePCH.h"

#include "SpecularMaterial.h"
#include "MaterialHelper.h"
#include "Core/Renderer/RayTracing/GetColorParameters.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/RayTracing/LightSource.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Renderer/RayTracing/Ray.h"
#include "Core/Renderer/RayTracing/LightSource.h"

namespace Muse
{

	SpecularMaterial::SpecularMaterial(glm::vec3 a_Color, float a_SpeculairStrength)
		: m_Color(a_Color), m_SpeculairStrength(a_SpeculairStrength)
	{
	}

	glm::vec3 SpecularMaterial::GetColor(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const
	{
		const glm::vec3 speculair = GetSpecular(a_RenderComponent, point, getColorParameters->RayDirection);
		const glm::vec3 combinedLights = speculair + a_RenderComponent->GetGameObject()->GetScene()->GetAmbientLight();
		const glm::vec3 result = m_Color * combinedLights;

		return result;
	}

	glm::vec3 SpecularMaterial::GetSpecular(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& a_Point, const glm::vec3& a_LookDirection) const
	{
		const std::vector<std::shared_ptr<LightSource>>& lightSources = a_RenderComponent->GetGameObject()->GetScene()->GetLightSources();

		glm::vec3 totalSpecular = glm::vec3(0);

		for (const std::shared_ptr<LightSource>& lightSource : lightSources)
		{
			const glm::vec3 lightPosition = lightSource->GetPosition();
			const glm::vec3 directionToLightSource = glm::normalize(lightPosition - a_Point);
			Ray ray{ a_Point, directionToLightSource };

			if (!ray.Cast())
			{
				float specular = GetSpecular(a_RenderComponent->GetNormal(a_Point), a_LookDirection, directionToLightSource);

				/*
				const glm::vec3 reflectionDirection = glm::reflect(directionToLightSource, a_RenderComponent->GetNormal(a_Point));
				const float specularValue = std::pow(std::max(glm::dot(a_LookDirection, reflectionDirection), 0.0f), 32.0f) * m_SpeculairStrength;
				const float clampedspeculairStrength = std::clamp(specularValue, 0.0f, 1.0f);
			    */

				totalSpecular += specular;
			}
		}

		return totalSpecular;
	}

    float SpecularMaterial::GetSpecular(const glm::vec3& a_Normal, const glm::vec3& a_LookDirection, const glm::vec3& a_DirectionToLightSource) const
    {
		const glm::vec3 reflectionDirection = glm::reflect(a_DirectionToLightSource, a_Normal);
		const float specularValue = std::pow(std::max(glm::dot(a_LookDirection, reflectionDirection), 0.0f), 32.0f) * m_SpeculairStrength;
		const float clampedspeculairStrength = std::clamp(specularValue, 0.0f, 1.0f);

		return clampedspeculairStrength;
    }
}
