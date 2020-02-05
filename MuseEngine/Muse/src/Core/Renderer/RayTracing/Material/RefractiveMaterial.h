#pragma once

#include "Material.h"
#include "BlinnPhongMaterial.h"


namespace Muse
{

	class SceneObject;

	class RefractiveMaterial : public BlinnPhongMaterial
	{
	public:
		RefractiveMaterial(const glm::vec3& a_Color = glm::vec3(1), float a_SpeculairStrength = 0.5f, float a_Refractiveness = 0.9f, float a_Eta = 0.975f, int a_MaxBounces = 32);
		virtual ~RefractiveMaterial() = default;

		virtual glm::vec3 GetColor(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, GetColorParameters& a_GetColorParameters) const override;
		glm::vec3 GetRefraction(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& a_Point, GetColorParameters& a_GetColorParameters) const;
		virtual const glm::vec3& GetColor() override { return m_Color; };

	private:
		int m_MaxBounces;
		float m_Refractiveness;
		float m_Eta;

	};

}