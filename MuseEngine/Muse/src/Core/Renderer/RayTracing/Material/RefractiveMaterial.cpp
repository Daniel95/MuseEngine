#include "MusePCH.h"

#include "RefractiveMaterial.h"
#include "MaterialHelper.h"
#include "DiffuseMaterial.h"
#include "SpeculairMaterial.h"
#include "Core/Renderer/RayTracing/GetColorParameters.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/RayTracing/RayHitData.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Renderer/RayTracing/Ray.h"

namespace Muse
{
	RefractiveMaterial::RefractiveMaterial(const glm::vec3& color, float speculairStrength, float refractiveness, float eta, int maxBounces)
		: BlinnPhongMaterial(color, speculairStrength), maxBounces(maxBounces), refractiveness(refractiveness), eta(eta)
	{
	}

	glm::vec3 RefractiveMaterial::GetColor(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const
	{
		const glm::vec3 speculair = m_SpeculairMaterial.GetSpeculair(a_RenderComponent, point, getColorParameters->RayDirection);
		const glm::vec3 diffuse = m_DiffuseMaterial.GetDiffuse(a_RenderComponent, point);
		const glm::vec3 blinnPhong = (speculair + diffuse + a_RenderComponent->GetGameObject()->GetScene()->GetAmbientLight()) * abs(1 - refractiveness);
		const glm::vec3 refraction = GetRefraction(a_RenderComponent, point, getColorParameters) * refractiveness;
		const glm::vec3 lightColor = blinnPhong + refraction;
		const glm::vec3 result = m_Color * lightColor;

		return result;
	}

	glm::vec3 RefractiveMaterial::GetRefraction(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const
	{
		if (getColorParameters->Bounces <= 0)
		{
			return a_RenderComponent->GetGameObject()->GetScene()->GetBackgroundColor();
		}
		getColorParameters->Bounces--;

		glm::vec3 refractionColor = a_RenderComponent->GetGameObject()->GetScene()->GetBackgroundColor();
		glm::vec3 refractionDirection = glm::refract(getColorParameters->RayDirection, a_RenderComponent->GetNormal(point), eta);

		std::vector<std::shared_ptr<RenderComponent>> renderComponents = RenderComponent::GetAll();
		renderComponents.erase(std::remove(renderComponents.begin(), renderComponents.end(), a_RenderComponent), renderComponents.end());

		std::vector<RayHitData> rayHitDatas;
		Ray ray;
		ray.Origin = point;
		ray.Direction = refractionDirection;

		if (a_RenderComponent->GetGameObject()->GetScene()->RayCast(rayHitDatas, renderComponents, ray))
		{
			getColorParameters->RayDirection = refractionDirection;
			int closestRayHitDataIndex = RayHitData::GetClosestRayHitDataIndex(rayHitDatas, point);
			RayHitData& closestHitData = rayHitDatas.at(closestRayHitDataIndex);
			refractionColor = closestHitData.m_RenderComponent->GetColor(closestHitData.m_IntersectionPoint, getColorParameters);
		}

		return refractionColor;
	}

}
