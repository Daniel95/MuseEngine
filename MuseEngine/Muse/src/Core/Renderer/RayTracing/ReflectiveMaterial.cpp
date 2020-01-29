#include "MusePCH.h"

#include "ReflectiveMaterial.h"
#include "SceneObject.h"
#include "Shape.h"
#include "Scene.h"
#include "RayHitData.h"
#include "Camera.h"
#include "MaterialHelper.h"
#include "LightSource.h"
#include "AmbientLightSource.h"
#include "DiffuseMaterial.h"
#include "SpeculairMaterial.h"
#include <cassert>

ReflectiveMaterial::ReflectiveMaterial(const sf::Color & color, float speculairStrength, float reflectiveness, int maxBounces)
	: BlinnPhongMaterial(color, speculairStrength), maxBounces(maxBounces), reflectiveness(reflectiveness)
{ }

ReflectiveMaterial::~ReflectiveMaterial() { }

sf::Color ReflectiveMaterial::GetColor(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const
{
	const sf::Color speculair = speculairMaterial.GetSpeculair(sceneObject, point, getColorParameters->RayDirection);
	const sf::Color diffuse = diffuseMaterial.GetDiffuse(sceneObject, point);
	const sf::Color blinnPhong = (speculair + diffuse + sceneObject.GetScene().GetAmbientLight()) * abs(1 - reflectiveness);
	const sf::Color reflection = GetReflection(sceneObject, point, getColorParameters) * reflectiveness;
	const sf::Color lightColor = blinnPhong + reflection;
	const sf::Color result = color * lightColor;

	return result;
}

sf::Color ReflectiveMaterial::GetReflection(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const
{
	if (getColorParameters->Bounces <= 0)
	{
		return sceneObject.GetScene().GetBackgroundColor();
	}
	getColorParameters->Bounces--;

	sf::Color reflectionColor = sceneObject.GetScene().GetBackgroundColor();
	const vec3 reflectionDirection = reflect(getColorParameters->RayDirection, sceneObject.GetNormal(point));

	std::vector<SceneObject*> sceneObjects = sceneObject.GetScene().GetSceneObjects();
	sceneObjects.erase(std::remove(sceneObjects.begin(), sceneObjects.end(), &sceneObject), sceneObjects.end());
	std::vector<std::shared_ptr<RayHitData>> rayHitDatas;
	std::shared_ptr<Ray> ray = std::make_shared<Ray>();
	ray->Origin = point;
	ray->Direction = reflectionDirection;

	if (sceneObject.GetScene().RayCast(rayHitDatas, sceneObjects, ray))
	{
		getColorParameters->RayDirection = reflectionDirection;
		const std::shared_ptr<RayHitData> closestHit = GetClosestRayHitData(rayHitDatas, point);
		reflectionColor = closestHit->HitSceneObject->GetColor(closestHit->IntersectionPoint, getColorParameters);
	}
	
	return reflectionColor;
}