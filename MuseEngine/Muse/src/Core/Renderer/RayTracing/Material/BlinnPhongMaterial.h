#pragma once

#include "Material.h"


namespace Muse
{

	struct GetColorParameters;
	class GameObject;
	class DiffuseMaterial;
	class SpeculairMaterial;

	class BlinnPhongMaterial : public Material
	{
	public:
		BlinnPhongMaterial(const glm::vec3& a_Color, float a_SpeculairStrength = 0.5f);
		virtual ~BlinnPhongMaterial() = default;
		virtual const glm::vec3 & GetColor(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& a_Point, std::shared_ptr<GetColorParameters> a_GetColorParameters) const override;

	protected:
		DiffuseMaterial& m_DiffuseMaterial;
		SpeculairMaterial& m_SpeculairMaterial;
		glm::vec3 m_Color;

	};

}