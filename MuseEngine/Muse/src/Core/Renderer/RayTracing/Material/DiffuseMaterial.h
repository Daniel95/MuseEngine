#pragma once

#include <glm/glm.hpp>

#include "Material.h"

namespace Muse
{
	class RenderComponent;

	class DiffuseMaterial : public Material
	{
	public:
		DiffuseMaterial(const glm::vec3& a_Color);
		virtual ~DiffuseMaterial() = default;

		virtual const glm::vec3 & GetColor(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const override;
		virtual const glm::vec3 & GetDiffuse(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point) const;
	private:
		glm::vec3 m_Color;

	};

}