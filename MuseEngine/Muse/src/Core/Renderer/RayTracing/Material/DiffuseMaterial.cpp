#include "MusePCH.h"

#include "Core/Gameplay/GameObject.h"

#include "DiffuseMaterial.h"
#include "MaterialHelper.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/RayTracing/LightSource.h"

namespace Muse
{

	DiffuseMaterial::DiffuseMaterial(const glm::vec3& a_Color)
    : m_Color(a_Color)
	{
	}

	const glm::vec3 & DiffuseMaterial::GetColor(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& a_Point, std::shared_ptr<GetColorParameters> a_GetColorParameters) const
	{
		a_GetColorParameters;

		const glm::vec3 diffuse = GetDiffuse(a_RenderComponent, a_Point);
		const glm::vec3 combinedLights = diffuse + a_RenderComponent->GetGameObject()->GetScene()->GetAmbientLight();
		const glm::vec3 result = m_Color * combinedLights;

		//return result;
		return m_Color;
	}

	const glm::vec3 & DiffuseMaterial::GetDiffuse(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point) const
	{
		const glm::vec3 normalDirection = a_RenderComponent->GetNormal(point);

		std::vector<LightSource*> lightSources = a_RenderComponent->GetGameObject()->GetScene()->GetLightSources();

		FilterBlockedLights(lightSources, a_RenderComponent, point);

		glm::vec3 totalDiffuse;

		for (LightSource* lightSource : lightSources)
		{
			const glm::vec3 lightPosition = lightSource->GetPosition();
			const glm::vec3 directionToLightSource = glm::normalize(lightPosition - point);
			const float diffuseStrength = glm::dot(directionToLightSource, normalDirection);
			const float clampedDiffuseStrength = std::clamp(diffuseStrength, 0.0f, 1.0f);
			const glm::vec3 lightColor = lightSource->GetLight(point) * clampedDiffuseStrength;

			totalDiffuse += lightColor;
		}

		return totalDiffuse;
	}

}
