#include "MusePCH.h"

#include "DiffuseMaterial.h"
#include "SceneObject.h"
#include "Shape.h"
#include "Scene.h"
#include "RayHitData.h"
#include "MaterialHelper.h"
#include "LightSource.h"
#include "AmbientLightSource.h"

DiffuseMaterial::DiffuseMaterial(const sf::Color & color)
	: Material(color) { }

DiffuseMaterial::~DiffuseMaterial() { }

sf::Color DiffuseMaterial::GetColor(const SceneObject& sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const
{
	getColorParameters;

	const sf::Color diffuse = GetDiffuse(sceneObject, point);
	const sf::Color combinedLights = diffuse + sceneObject.GetScene().GetAmbientLight();
	const sf::Color result = color * combinedLights;

	return result;
}

sf::Color DiffuseMaterial::GetDiffuse(const SceneObject& sceneObject, const vec3 & point) const
{
	const vec3 normalDirection = sceneObject.GetNormal(point);

	std::vector<LightSource*> lightSources = sceneObject.GetScene().GetLightSources();

	FilterBlockedLights(lightSources, sceneObject, point);

	sf::Color totalDiffuse;

	for (LightSource* lightSource : lightSources)
	{
		const vec3 lightPosition = lightSource->GetPosition();
		const vec3 directionToLightSource = (lightPosition - point).normalized();
		const float diffuseStrength = directionToLightSource.dot(normalDirection);
		const float clampedDiffuseStrength = std::clamp(diffuseStrength, 0.0f, 1.0f);
		const sf::Color lightColor = lightSource->GetLight(point) * clampedDiffuseStrength;

		totalDiffuse += lightColor;
	}


	return totalDiffuse;
}