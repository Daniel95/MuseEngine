#pragma once

#include "Material.h"
#include "BlinnPhongMaterial.h"

namespace Muse
{
	class GameObject;

	class ReflectiveMaterial : public BlinnPhongMaterial
	{
	public:
		ReflectiveMaterial(const glm::vec3& a_Color = glm::vec3(1), float a_SpeculairStrength = 0.5f, float a_Reflectiveness = 0.75f, int a_MaxBounces = 32);
		virtual ~ReflectiveMaterial() = default;

		virtual glm::vec3 GetColor(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, GetColorParameters& a_GetColorParameters) const override;
		glm::vec3 GetReflection(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, GetColorParameters& a_GetColorParameters) const;

	private:
		int maxBounces;
		float reflectiveness;

	};
}