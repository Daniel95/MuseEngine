#include "MusePCH.h"

#include "DiffuseMaterial.h"
#include "SceneObject.h"
#include "Shape.h"
#include "RayHitData.h"
#include "MaterialHelper.h"
#include "LightSource.h"
#include "AmbientLightSource.h"

DiffuseMaterial::DiffuseMaterial(const glm::vec3 & color)
	: Material(color) { }

DiffuseMaterial::~DiffuseMaterial() { }

glm::vec3 DiffuseMaterial::GetColor(const SceneObject& sceneObject, const glm::vec3 & a_Point, std::shared_ptr<GetColorParameters> a_GetColorParameters) const
{
	a_GetColorParameters;

	const glm::vec3 diffuse = GetDiffuse(sceneObject, a_Point);
	const glm::vec3 combinedLights = diffuse + sceneObject.GetScene().GetAmbientLight();
	const glm::vec3 result = color * combinedLights;

	return result;
}

glm::vec3 DiffuseMaterial::GetDiffuse(const SceneObject& sceneObject, const glm::vec3 & point) const
{
	const glm::vec3 normalDirection = sceneObject.GetNormal(point);

	std::vector<LightSource*> lightSources = sceneObject.GetScene().GetLightSources();

	FilterBlockedLights(lightSources, sceneObject, point);

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