#include "MusePCH.h"

#include "Core/Gameplay/GameObject.h"

#include "DiffuseMaterial.h"
#include "MaterialHelper.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/RayTracing/LightSource.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Renderer/RayTracing/Shape/Shape.h"
#include "Core/Renderer/RayTracing/Ray.h"

namespace Muse
{
	DiffuseMaterial::DiffuseMaterial(const glm::vec3& a_Color)
    : m_Color(a_Color)
	{
	}

	glm::vec3 DiffuseMaterial::GetColor(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& a_Point, std::shared_ptr<GetColorParameters> a_GetColorParameters) const
	{
		a_GetColorParameters;

		const glm::vec3 diffuse = GetDiffuse(a_RenderComponent, a_Point);
		const glm::vec3 combinedLights = diffuse + SceneManager::GetActiveScene()->GetAmbientLight();
		const glm::vec3 result = m_Color * combinedLights;

		return result;
	}

	glm::vec3 DiffuseMaterial::GetDiffuse(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& a_Point) const
	{
		const glm::vec3 normal = a_RenderComponent->GetNormal(a_Point);

		const std::vector<std::shared_ptr<LightSource>>& lightSources = SceneManager::GetActiveScene()->GetLightSources();

		glm::vec3 totalDiffuse = glm::vec3(0);

		for(const std::shared_ptr<LightSource>& lightSource : lightSources)
		{
			const glm::vec3 lightPosition = lightSource->GetPosition();
			const glm::vec3 directionToLightSource = glm::normalize(lightPosition - a_Point);
			Ray ray{ a_Point, directionToLightSource };

			if (!ray.Cast())
			{
				const glm::vec3 directionToLightSource = glm::normalize(lightPosition - a_Point);
				const float diffuseStrength = glm::dot(directionToLightSource, normal);
				const float clampedDiffuseStrength = std::clamp(diffuseStrength, 0.0f, 1.0f);
				const glm::vec3 lightColor = lightSource->GetLight(a_Point) * clampedDiffuseStrength;

				totalDiffuse += lightColor;
			}
		}

		return totalDiffuse;
	}

	float DiffuseMaterial::GetDiffuse(const glm::vec3& a_Normal, const glm::vec3& a_DirectionToLightSource)
	{
		const float diffuseStrength = glm::dot(a_DirectionToLightSource, a_Normal);
		const float clampedDiffuseStrength = std::clamp(diffuseStrength, 0.0f, 1.0f);

		return clampedDiffuseStrength;
	}
}
