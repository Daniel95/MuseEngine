#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

namespace Muse
{
    class RenderComponent;

	struct RayHitData
	{
		//RayHitData() = default;
		//RayHitData(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3 & a_InterSectionPoint);

		static int GetClosestRayHitDataIndex(const std::vector<RayHitData>& a_RayHitDatas, const glm::vec3& a_RayOrigin);
		static void RemoveRayHitsOutOfDistance(std::vector<RayHitData>& a_RayHitDatas, const glm::vec3& a_RayOrigin, float a_MaxDistance);

		std::shared_ptr<const RenderComponent> m_RenderComponent;
		glm::vec3 m_IntersectionPoint;
	};
}