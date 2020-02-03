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
#include "Core/Gameplay/Component/RenderComponent.h"

namespace Muse
{

	ReflectiveMaterial::ReflectiveMaterial(const glm::vec3& color, float speculairStrength, float reflectiveness, int maxBounces)
		: BlinnPhongMaterial(color, speculairStrength), maxBounces(maxBounces), reflectiveness(reflectiveness)
	{
	}

	ReflectiveMaterial::~ReflectiveMaterial() {}

	glm::vec3 ReflectiveMaterial::GetColor(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const
	{
		const glm::vec3 speculair = m_SpeculairMaterial.GetSpeculair(a_RenderComponent, point, getColorParameters->RayDirection);
		const glm::vec3 diffuse = m_DiffuseMaterial.GetDiffuse(a_RenderComponent, point);
		const glm::vec3 blinnPhong = (speculair + diffuse + a_RenderComponent->GetGameObject()->GetScene()->GetAmbientLight()) * abs(1 - reflectiveness);
		const glm::vec3 reflection = GetReflection(a_RenderComponent, point, getColorParameters) * reflectiveness;
		const glm::vec3 lightColor = blinnPhong + reflection;
		const glm::vec3 result = m_Color * lightColor;

		return result;
	}

	glm::vec3 ReflectiveMaterial::GetReflection(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const
	{
		if (getColorParameters->Bounces <= 0)
		{
			return a_RenderComponent->GetGameObject()->GetScene()->GetBackgroundColor();
		}
		getColorParameters->Bounces--;

		glm::vec3 reflectionColor = a_RenderComponent->GetGameObject()->GetScene()->GetBackgroundColor();
		const glm::vec3 reflectionDirection = glm::reflect(getColorParameters->RayDirection, a_RenderComponent->GetNormal(point));

		std::vector<std::shared_ptr<RenderComponent>> renderComponents = RenderComponent::GetAll();
		renderComponents.erase(std::remove(renderComponents.begin(), renderComponents.end(), a_RenderComponent), renderComponents.end());
		std::vector<std::shared_ptr<RayHitData>> rayHitDatas;
		std::shared_ptr<Ray> ray = std::make_shared<Ray>();
		ray->Origin = point;
		ray->Direction = reflectionDirection;

		if (a_RenderComponent->GetGameObject()->GetScene()->RayCast(rayHitDatas, renderComponents, ray))
		{
			getColorParameters->RayDirection = reflectionDirection;
			const std::shared_ptr<RayHitData> closestHit = GetClosestRayHitData(rayHitDatas, point);
			reflectionColor = closestHit->m_RenderComponent->GetColor(closestHit->m_IntersectionPoint, getColorParameters);
		}

		return reflectionColor;
	}

}
