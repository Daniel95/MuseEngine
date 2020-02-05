#include "MusePCH.h"

#include "RefractiveMaterial.h"
#include "MaterialHelper.h"
#include "DiffuseMaterial.h"
#include "SpecularMaterial.h"
#include "Core/Renderer/RayTracing/GetColorParameters.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/RayTracing/RayHitData.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Renderer/RayTracing/Ray.h"

namespace Muse
{
	RefractiveMaterial::RefractiveMaterial(const glm::vec3& a_Color, float a_SpeculairStrength, float a_Refractiveness, float a_Eta, int a_MaxBounces)
		: BlinnPhongMaterial(a_Color, a_SpeculairStrength), m_MaxBounces(a_MaxBounces), m_Refractiveness(a_Refractiveness), m_Eta(a_Eta)
	{
	}

	glm::vec3 RefractiveMaterial::GetColor(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, GetColorParameters& a_GetColorParameters) const
	{
		const glm::vec3 speculair = m_SpeculairMaterial.GetSpecular(a_RenderComponent, a_Point, a_GetColorParameters.RayDirection);
		const glm::vec3 diffuse = m_DiffuseMaterial.GetDiffuse(a_RenderComponent, a_Point);
		const glm::vec3 blinnPhong = (speculair + diffuse + a_RenderComponent->GetGameObject()->GetScene()->GetAmbientLight()) * abs(1 - m_Refractiveness);
		const glm::vec3 refraction = GetRefraction(a_RenderComponent, a_Point, a_GetColorParameters) * m_Refractiveness;
		const glm::vec3 lightColor = blinnPhong + refraction;
		const glm::vec3 result = m_Color * lightColor;

		return result;
	}

	glm::vec3 RefractiveMaterial::GetRefraction(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& a_Point, GetColorParameters& a_GetColorParameters) const
	{
		if (a_GetColorParameters.Bounces <= 0)
		{
			return a_RenderComponent->GetGameObject()->GetScene()->GetBackgroundColor();
		}
		a_GetColorParameters.Bounces--;

		glm::vec3 refractionColor = a_RenderComponent->GetGameObject()->GetScene()->GetBackgroundColor();
		glm::vec3 refractionDirection = glm::refract(a_GetColorParameters.RayDirection, a_RenderComponent->GetNormal(a_Point), m_Eta);

		std::vector<std::shared_ptr<RenderComponent>> renderComponents = RenderComponent::GetAll();
		renderComponents.erase(std::remove(renderComponents.begin(), renderComponents.end(), a_RenderComponent), renderComponents.end());

		RayHitData rayHitData;
		Ray ray{ a_Point, refractionDirection };

		if (ray.Cast(rayHitData, a_RenderComponent))
		{
			a_GetColorParameters.RayDirection = refractionDirection;
			rayHitData.UpdateIntersectionPoint(ray);
			refractionColor = rayHitData.m_RenderComponent->GetColor(rayHitData.GetIntersectionPoint(), a_GetColorParameters);
		}

		return refractionColor;
	}

}
