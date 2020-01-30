#include "MusePCH.h"

#include "ReflectiveMaterial.h"
#include "MaterialHelper.h"
#include "DiffuseMaterial.h"
#include "SpeculairMaterial.h"
#include <cassert>
#include "Core/Renderer/RayTracing/GetColorParameters.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Renderer/RayTracing/RayHitData.h"
#include "Core/Renderer/RayTracing/Ray.h"
#include "Core/Scene/Scene.h"

namespace Muse
{

	ReflectiveMaterial::ReflectiveMaterial(const glm::vec3& color, float speculairStrength, float reflectiveness, int maxBounces)
		: BlinnPhongMaterial(color, speculairStrength), maxBounces(maxBounces), reflectiveness(reflectiveness)
	{
	}

	ReflectiveMaterial::~ReflectiveMaterial() {}

	glm::vec3 ReflectiveMaterial::GetColor(const GameObject& sceneObject, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const
	{
		const glm::vec3 speculair = m_SpeculairMaterial.GetSpeculair(sceneObject, point, getColorParameters->RayDirection);
		const glm::vec3 diffuse = m_DiffuseMaterial.GetDiffuse(sceneObject, point);
		const glm::vec3 blinnPhong = (speculair + diffuse + sceneObject.GetScene()->GetAmbientLight()) * abs(1 - reflectiveness);
		const glm::vec3 reflection = GetReflection(sceneObject, point, getColorParameters) * reflectiveness;
		const glm::vec3 lightColor = blinnPhong + reflection;
		const glm::vec3 result = m_Color * lightColor;

		return result;
	}

	glm::vec3 ReflectiveMaterial::GetReflection(const GameObject& a_GameObject, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const
	{
		if (getColorParameters->Bounces <= 0)
		{
			return a_GameObject.GetScene()->GetBackgroundColor();
		}
		getColorParameters->Bounces--;

		glm::vec3 reflectionColor = a_GameObject.GetScene()->GetBackgroundColor();
		const glm::vec3 reflectionDirection = glm::reflect(getColorParameters->RayDirection, a_GameObject.GetNormal(point);

		std::vector<SceneObject*> sceneObjects = a_GameObject.GetScene()->GetSceneObjects();
		sceneObjects.erase(std::remove(sceneObjects.begin(), sceneObjects.end(), &a_GameObject), sceneObjects.end());
		std::vector<std::shared_ptr<RayHitData>> rayHitDatas;
		std::shared_ptr<Ray> ray = std::make_shared<Ray>();
		ray->Origin = point;
		ray->Direction = reflectionDirection;

		if (a_GameObject.GetScene()->RayCast(rayHitDatas, sceneObjects, ray))
		{
			getColorParameters->RayDirection = reflectionDirection;
			const std::shared_ptr<RayHitData> closestHit = GetClosestRayHitData(rayHitDatas, point);
			reflectionColor = closestHit->HitSceneObject->GetColor(closestHit->IntersectionPoint, getColorParameters);
		}

		return reflectionColor;
	}

}
