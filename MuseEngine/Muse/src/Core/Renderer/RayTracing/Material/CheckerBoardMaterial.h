#pragma once

#include "Material.h"
#include "BlinnPhongMaterial.h"

namespace Muse
{
	class GameObject;

	class CheckerBoardMaterial : public BlinnPhongMaterial
	{
	public:
		CheckerBoardMaterial(const glm::vec3& a_Color1 = glm::vec3(0.f, 0.f, 1.f), const glm::vec3& a_Color2 = glm::vec3(1.f, 0.f, 0.f), float a_GridSize = 5, float a_SpeculairStrength = 0.5f);
		virtual~CheckerBoardMaterial() = default;
		virtual glm::vec3 GetColor(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& a_Point, std::shared_ptr<GetColorParameters> a_GetColorParameters) const override;

	protected:
		glm::vec3 m_Color2;
		float m_GridSize;

	};

}