#pragma once

#include <glm/glm.hpp>

namespace Muse
{
    class RenderComponent;
    struct GetColorParameters;

	class Material
	{
	public:
		virtual ~Material() = default;
		virtual glm::vec3 GetColor(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, GetColorParameters& a_GetColorParameters) const = 0;
		virtual const glm::vec3& GetColor() = 0;

	};
}