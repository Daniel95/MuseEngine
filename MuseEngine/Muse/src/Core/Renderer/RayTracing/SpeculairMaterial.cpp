#include "MusePCH.h"

#include "SpeculairMaterial.h"
#include "SceneObject.h"
#include "Shape.h"
#include "RayHitData.h"
#include "Camera.h"
#include "MaterialHelper.h"
#include "LightSource.h"
#include "AmbientLightSource.h"

SpeculairMaterial::SpeculairMaterial(sf::Color color, float speculairStrength)
	: Material(color), speculairStrength(speculairStrength) { }

SpeculairMaterial::~SpeculairMaterial() { }

sf::Color SpeculairMaterial::GetColor(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const
{
	const sf::Color speculair = GetSpeculair(sceneObject, point, getColorParameters->RayDirection);
	const sf::Color combinedLights = speculair + sceneObject.GetScene().GetAmbientLight();
	const sf::Color result = color * combinedLights;

	return result;
}

const sf::Color SpeculairMaterial::GetSpeculair(const SceneObject & sceneObject, const vec3 & point, const vec3 & lookDirection) const
{
	std::vector<LightSource*> lights = sceneObject.GetScene().GetLightSources();

	FilterBlockedLights(lights, sceneObject, point);

	sf::Color totalSpeculair;

	for (LightSource* light : lights)
	{
		const vec3 lightDirection = light->GetDirectionToPoint(point);
		const vec3 reflectionDirection = reflect(lightDirection * -1, sceneObject.GetNormal(point));
		const float speculairValue = std::pow(std::max(dot(lookDirection, reflectionDirection), 0.0f), 32.0f) * speculairStrength;
		const float clampedspeculairStrength = std::clamp(speculairValue, 0.0f, 1.0f);

		totalSpeculair += light->GetLight(point) * clampedspeculairStrength;
	}

	return totalSpeculair;
}

