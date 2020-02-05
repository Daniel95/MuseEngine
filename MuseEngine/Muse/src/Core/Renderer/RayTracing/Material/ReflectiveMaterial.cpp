#include "MusePCH.h"

#include "ReflectiveMaterial.h"
#include "MaterialHelper.h"
#include "DiffuseMaterial.h"
#include "SpecularMaterial.h"
#include <cassert>
#include "Core/Renderer/RayTracing/GetColorParameters.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Renderer/RayTracing/RayHitData.h"
#include "Core/Renderer/RayTracing/Ray.h"
#include "Core/Scene/Scene.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Scene/SceneManager.h"

namespace Muse
{

	ReflectiveMaterial::ReflectiveMaterial(const glm::vec3& a_Color, float a_SpeculairStrength, float a_Reflectiveness, int a_MaxBounces)
		: BlinnPhongMaterial(a_Color, a_SpeculairStrength), maxBounces(a_MaxBounces), reflectiveness(a_Reflectiveness)
	{
	}

	glm::vec3 ReflectiveMaterial::GetColor(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, const std::shared_ptr<GetColorParameters>& a_GetColorParameters) const
	{
		const glm::vec3 speculair = m_SpeculairMaterial.GetSpecular(a_RenderComponent, a_Point, a_GetColorParameters->RayDirection);
		const glm::vec3 diffuse = m_DiffuseMaterial.GetDiffuse(a_RenderComponent, a_Point);
		const glm::vec3 blinnPhong = (speculair + diffuse + a_RenderComponent->GetGameObject()->GetScene()->GetAmbientLight()) * abs(1 - reflectiveness);
		const glm::vec3 reflection = GetReflection(a_RenderComponent, a_Point, a_GetColorParameters) * reflectiveness;
		const glm::vec3 lightColor = blinnPhong + reflection;
		const glm::vec3 result = m_Color * lightColor;

		return result;
	}

	glm::vec3 ReflectiveMaterial::GetReflection(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, const std::shared_ptr<GetColorParameters>& a_GetColorParameters) const
	{
		if (a_GetColorParameters->Bounces <= 0)
		{
			return SceneManager::GetActiveScene()->GetBackgroundColor();
		}
		a_GetColorParameters->Bounces--;

		glm::vec3 reflectionColor = SceneManager::GetActiveScene()->GetBackgroundColor();
		const glm::vec3 reflectionDirection = glm::reflect(a_GetColorParameters->RayDirection, a_RenderComponent->GetNormal(a_Point));

		std::vector<std::shared_ptr<RenderComponent>> renderComponents = RenderComponent::GetAll();
		renderComponents.erase(std::remove(renderComponents.begin(), renderComponents.end(), a_RenderComponent), renderComponents.end());
		RayHitData rayHitData;
		Ray ray{ a_Point, reflectionDirection };

		if(ray.Cast(rayHitData, a_RenderComponent))
		{
			a_GetColorParameters->RayDirection = reflectionDirection;
			rayHitData.UpdateIntersectionPoint(ray);

			reflectionColor = rayHitData.m_RenderComponent->GetColor(rayHitData.GetIntersectionPoint(), a_GetColorParameters);
		}

		return reflectionColor;
	}
}
