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

		virtual glm::vec3 GetColor(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const override;
		glm::vec3 GetSpecular(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& a_Point, const glm::vec3& a_LookDirection) const;
		float GetSpecular(const glm::vec3& a_Normal, const glm::vec3& a_LookDirection, const glm::vec3& a_DirectionToLightSource) const;

	private:
		float m_SpeculairStrength;
		glm::vec3 m_Color;

	};
}
