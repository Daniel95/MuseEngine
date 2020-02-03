#include "MusePCH.h"

#include "SpeculairMaterial.h"
#include "MaterialHelper.h"
#include "Core/Renderer/RayTracing/GetColorParameters.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/RayTracing/LightSource.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Gameplay/GameObject.h"


namespace Muse
{

	SpeculairMaterial::SpeculairMaterial(glm::vec3 color, float speculairStrength)
		: m_Color(color), m_SpeculairStrength(speculairStrength)
	{
	}

	glm::vec3 SpeculairMaterial::GetColor(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const
	{
		const glm::vec3 speculair = GetSpeculair(a_RenderComponent, point, getColorParameters->RayDirection);
		const glm::vec3 combinedLights = speculair + a_RenderComponent->GetGameObject()->GetScene()->GetAmbientLight();
		const glm::vec3 result = m_Color * combinedLights;

		return result;
	}

	glm::vec3 SpeculairMaterial::GetSpeculair(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point, const glm::vec3& lookDirection) const
	{
		std::vector<std::shared_ptr<LightSource>> lights = a_RenderComponent->GetGameObject()->GetScene()->GetLightSources();

		FilterBlockedLights(lights, a_RenderComponent, point);

		glm::vec3 totalSpeculair;

		for (std::shared_ptr<LightSource> light : lights)
		{
			const glm::vec3 lightDirection = light->GetDirectionToPoint(point);
			const glm::vec3 reflectionDirection = reflect(lightDirection * -1, a_RenderComponent->GetNormal(point));
			const float speculairValue = std::pow(std::max(dot(lookDirection, reflectionDirection), 0.0f), 32.0f) * m_SpeculairStrength;
			const float clampedspeculairStrength = std::clamp(speculairValue, 0.0f, 1.0f);

			totalSpeculair += light->GetLight(point) * clampedspeculairStrength;
		}

		return totalSpeculair;
	}

}
