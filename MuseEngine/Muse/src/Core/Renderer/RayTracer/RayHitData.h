#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

namespace Muse
{
	class RenderComponent;
	struct Ray;

	class RayHitData
	{
	public:
		std::shared_ptr<const RenderComponent> m_RenderComponent;
		float m_Distance;

		const glm::vec3& GetIntersectionPoint() const;
		const glm::vec3& UpdateIntersectionPoint(const Ray& a_Ray);

	private:
		glm::vec3 m_IntersectionPoint = glm::vec3(std::numeric_limits<float>::max());

	};
}