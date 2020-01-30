#include "MusePCH.h"

#include "SpeculairMaterial.h"
#include "MaterialHelper.h"
#include "Core/Renderer/RayTracing/GetColorParameters.h"
#include "Core/Renderer/RayTracing/SceneObject.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/RayTracing/LightSource.h"


namespace Muse
{

	SpeculairMaterial::SpeculairMaterial(glm::vec3 color, float speculairStrength)
		: m_Color(color), m_SpeculairStrength(speculairStrength)
	{
	}

	SpeculairMaterial::~SpeculairMaterial() {}

	glm::vec3 SpeculairMaterial::GetColor(const GameObject& sceneObject, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const
	{
		const glm::vec3 speculair = GetSpeculair(sceneObject, point, getColorParameters->RayDirection);
		const glm::vec3 combinedLights = speculair + sceneObject.GetScene().GetAmbientLight();
		const glm::vec3 result = m_Color * combinedLights;

		return result;
	}

	const glm::vec3 SpeculairMaterial::GetSpeculair(const GameObject& sceneObject, const glm::vec3& point, const glm::vec3& lookDirection) const
	{
		std::vector<LightSource*> lights = sceneObject.GetScene().GetLightSources();

		FilterBlockedLights(lights, sceneObject, point);

		glm::vec3 totalSpeculair;

		for (LightSource* light : lights)
		{
			const glm::vec3 lightDirection = light->GetDirectionToPoint(point);
			const glm::vec3 reflectionDirection = reflect(lightDirection * -1, sceneObject.GetNormal(point));
			const float speculairValue = std::pow(std::max(dot(lookDirection, reflectionDirection), 0.0f), 32.0f) * m_SpeculairStrength;
			const float clampedspeculairStrength = std::clamp(speculairValue, 0.0f, 1.0f);

			totalSpeculair += light->GetLight(point) * clampedspeculairStrength;
		}

		return totalSpeculair;
	}

}