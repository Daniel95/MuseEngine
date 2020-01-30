#include "MusePCH.h"

#include "SpeculairMaterial.h"
#include "SceneObject.h"
#include "Shape.h"
#include "RayHitData.h"
#include "Camera.h"
#include "MaterialHelper.h"
#include "LightSource.h"
#include "AmbientLightSource.h"

SpeculairMaterial::SpeculairMaterial(glm::vec3 color, float speculairStrength)
	: Material(color), speculairStrength(speculairStrength) { }

SpeculairMaterial::~SpeculairMaterial() { }

glm::vec3 SpeculairMaterial::GetColor(const SceneObject & sceneObject, const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const
{
	const glm::vec3 speculair = GetSpeculair(sceneObject, point, getColorParameters->RayDirection);
	const glm::vec3 combinedLights = speculair + sceneObject.GetScene().GetAmbientLight();
	const glm::vec3 result = color * combinedLights;

	return result;
}

const glm::vec3 SpeculairMaterial::GetSpeculair(const SceneObject & sceneObject, const glm::vec3 & point, const glm::vec3 & lookDirection) const
{
	std::vector<LightSource*> lights = sceneObject.GetScene().GetLightSources();

	FilterBlockedLights(lights, sceneObject, point);

	glm::vec3 totalSpeculair;

	for (LightSource* light : lights)
	{
		const glm::vec3 lightDirection = light->GetDirectionToPoint(point);
		const glm::vec3 reflectionDirection = reflect(lightDirection * -1, sceneObject.GetNormal(point));
		const float speculairValue = std::pow(std::max(dot(lookDirection, reflectionDirection), 0.0f), 32.0f) * speculairStrength;
		const float clampedspeculairStrength = std::clamp(speculairValue, 0.0f, 1.0f);

		totalSpeculair += light->GetLight(point) * clampedspeculairStrength;
	}

	return totalSpeculair;
}

