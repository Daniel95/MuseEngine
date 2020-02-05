#pragma once

#include "Material.h"

#include <glm/glm.hpp>

namespace Muse
{
	class GameObject;

	class NormalDebugMaterial : public Material
	{
	public:
		NormalDebugMaterial(const glm::vec3& a_Color = glm::vec3(1.f, 1.f, 1.f));
		virtual ~NormalDebugMaterial() = default;

		virtual glm::vec3 GetColor(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, GetColorParameters& a_GetColorParameters) const override;
		virtual const glm::vec3& GetColor() override { return m_Color; };

	private:
		glm::vec3 m_Color;

	};
}