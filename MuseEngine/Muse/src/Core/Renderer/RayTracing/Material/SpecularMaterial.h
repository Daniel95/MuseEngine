#pragma once

#include "Material.h"

#include <glm/glm.hpp>

namespace Muse
{
    class LightSource;
    class SceneObject;

	class SpecularMaterial : public Material
	{
	public:
		SpecularMaterial(glm::vec3 a_Color = glm::vec3(1), float a_SpeculairStrength = 0.5f);
		virtual ~SpecularMaterial() = default;

		virtual glm::vec3 GetColor(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, const std::shared_ptr<GetColorParameters>& a_GetColorParameters) const override;
		glm::vec3 GetSpecular(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, const glm::vec3& a_LookDirection) const;
		float GetSpecular(const glm::vec3& a_Normal, const glm::vec3& a_LookDirection, const glm::vec3& a_DirectionToLightSource) const;

	private:
		float m_SpeculairStrength;
		glm::vec3 m_Color;

	};
}
