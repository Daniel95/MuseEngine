#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

namespace Muse
{
    class RenderComponent;

	struct RayHitData
	{
		RayHitData(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3 & interSectionPoint);

		std::shared_ptr<const RenderComponent> m_RenderComponent;
		const glm::vec3 m_IntersectionPoint;
	};

	const std::shared_ptr<RayHitData> GetClosestRayHitData(std::vector<std::shared_ptr<RayHitData>> rayHitDatas, glm::vec3 rayOrigin);
	const void RemoveRayHitsOutOfDistance(std::vector<std::shared_ptr<RayHitData>>& rayHitDatas, glm::vec3 rayOrigin, float maxDistance);

}