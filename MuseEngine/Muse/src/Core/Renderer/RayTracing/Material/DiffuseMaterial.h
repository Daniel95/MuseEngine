#pragma once

#include <glm/glm.hpp>

#include "Material.h"

namespace Muse
{
	class RenderComponent;
	class AmbientLight;

	class DiffuseMaterial : public Material
	{
	public:
		DiffuseMaterial(const glm::vec3& a_Color);
		virtual ~DiffuseMaterial() = default;

		virtual glm::vec3 GetColor(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, GetColorParameters& a_GetColorParameters) const override;
		virtual glm::vec3 GetDiffuse(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& a_Point) const;
		static float GetDiffuse(const glm::vec3& a_Normal, const glm::vec3& a_DirectionToLightSource);

	private:
		glm::vec3 m_Color;
		std::shared_ptr<AmbientLight> m_AmbientLight;

	};

}