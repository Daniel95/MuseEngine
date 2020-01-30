#include "MusePCH.h"

#include "Core/Gameplay/GameObject.h"

#include "DiffuseMaterial.h"
#include "SceneObject.h"
#include "Shape.h"
#include "RayHitData.h"
#include "MaterialHelper.h"
#include "LightSource.h"
#include "AmbientLightSource.h"


namespace Muse
{

	DiffuseMaterial::DiffuseMaterial(const glm::vec3& color)
		: Material(color)
	{
	}

	DiffuseMaterial::~DiffuseMaterial() {}

	glm::vec3 DiffuseMaterial::GetColor(const GameObject& a_GameObject, const glm::vec3& a_Point, std::shared_ptr<GetColorParameters> a_GetColorParameters) const
	{
		a_GetColorParameters;

		const glm::vec3 diffuse = GetDiffuse(a_GameObject, a_Point);
		const glm::vec3 combinedLights = diffuse + a_GameObject.GetScene().GetAmbientLight();
		const glm::vec3 result = m_Color * combinedLights;

		return result;
	}

	glm::vec3 DiffuseMaterial::GetDiffuse(const GameObject& a_GameObject, const glm::vec3& point) const
	{
		const glm::vec3 normalDirection = a_GameObject.GetNormal(point);

		std::vector<LightSource*> lightSources = a_GameObject.GetScene().GetLightSources();

		FilterBlockedLights(lightSources, a_GameObject, point);

		glm::vec3 totalDiffuse;

		for (LightSource* lightSource : lightSources)
		{
			const glm::vec3 lightPosition = lightSource->GetPosition();
			const glm::vec3 directionToLightSource = (lightPosition - point).normalized();
			const float diffuseStrength = directionToLightSource.dot(normalDirection);
			const float clampedDiffuseStrength = std::clamp(diffuseStrength, 0.0f, 1.0f);
			const glm::vec3 lightColor = lightSource->GetLight(point) * clampedDiffuseStrength;

			totalDiffuse += lightColor;
		}


		return totalDiffuse;
	}

}