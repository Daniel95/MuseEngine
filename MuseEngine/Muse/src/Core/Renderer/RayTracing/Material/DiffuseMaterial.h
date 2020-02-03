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

		virtual glm::vec3 GetColor(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& a_Point, std::shared_ptr<GetColorParameters> a_GetColorParameters) const override;
		virtual glm::vec3 GetDiffuse(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& a_Point) const;
	private:
		glm::vec3 m_Color;
		std::shared_ptr<AmbientLight> m_AmbientLight;

	};

}