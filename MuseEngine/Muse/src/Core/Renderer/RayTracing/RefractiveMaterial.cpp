#include "MusePCH.h"

#include "RefractiveMaterial.h"
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
#include "Ray.h"
#include <cassert>

RefractiveMaterial::RefractiveMaterial(const glm::vec3 & color, float speculairStrength, float refractiveness, float eta, int maxBounces)
	: BlinnPhongMaterial(color, speculairStrength), maxBounces(maxBounces), refractiveness(refractiveness), eta(eta)
{}

RefractiveMaterial::~RefractiveMaterial() {}

glm::vec3 RefractiveMaterial::GetColor(const SceneObject & sceneObject, const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const
{
	const glm::vec3 speculair = speculairMaterial.GetSpeculair(sceneObject, point, getColorParameters->RayDirection);
	const glm::vec3 diffuse = diffuseMaterial.GetDiffuse(sceneObject, point);
	const glm::vec3 blinnPhong = (speculair + diffuse + sceneObject.GetScene().GetAmbientLight()) * abs(1 - refractiveness);
	const glm::vec3 refraction = GetRefraction(sceneObject, point, getColorParameters) * refractiveness;
	const glm::vec3 lightColor = blinnPhong + refraction;
	const glm::vec3 result = color * lightColor;

	return result;
}

glm::vec3 RefractiveMaterial::GetRefraction(const SceneObject & sceneObject, const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const
{
	if (getColorParameters->Bounces <= 0)
	{
		return sceneObject.GetScene().GetBackgroundColor();
	}
	getColorParameters->Bounces--;

	glm::vec3 refractionColor = sceneObject.GetScene().GetBackgroundColor();
    glm::vec3 refractionDirection = refract(getColorParameters->RayDirection, sceneObject.GetNormal(point), eta);

	std::vector<SceneObject*> sceneObjects = sceneObject.GetScene().GetSceneObjects();
	sceneObjects.erase(std::remove(sceneObjects.begin(), sceneObjects.end(), &sceneObject), sceneObjects.end());

	std::vector<std::shared_ptr<RayHitData>> rayHitDatas;
	std::shared_ptr<Ray> ray = std::make_shared<Ray>();
	ray->Origin = point;
	ray->Direction = refractionDirection;

	if (sceneObject.GetScene().RayCast(rayHitDatas, sceneObjects, ray))
	{
		getColorParameters->RayDirection = refractionDirection;
		const std::shared_ptr<RayHitData> closestHit = GetClosestRayHitData(rayHitDatas, point);
		refractionColor = closestHit->HitSceneObject->GetColor(closestHit->IntersectionPoint, getColorParameters);
	}

	return refractionColor;
}