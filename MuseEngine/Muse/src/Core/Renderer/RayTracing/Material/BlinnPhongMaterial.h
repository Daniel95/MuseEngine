#pragma once

#include "Material.h"


namespace Muse
{

	struct GetColorParameters;
	class GameObject;
	class DiffuseMaterial;
	class SpecularMaterial;

	class BlinnPhongMaterial : public Material
	{
	public:
		BlinnPhongMaterial(const glm::vec3& a_Color, float a_SpeculairStrength = 0.5f);
		virtual ~BlinnPhongMaterial() = default;
		virtual glm::vec3 GetColor(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, GetColorParameters& a_GetColorParameters) const override;
		glm::vec3 GetBlinnPhong(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, GetColorParameters& a_GetColorParameters) const;

	protected:
		DiffuseMaterial& m_DiffuseMaterial;
		SpecularMaterial& m_SpeculairMaterial;
		glm::vec3 m_Color;

	};

}