#include "MusePCH.h"

#include "RayHitData.h"
#include "Core/Gameplay/Component/RenderComponent.h"

namespace Muse
{
    RayHitData::RayHitData(std::shared_ptr<const RenderComponent> a_RenderComponent,
		const glm::vec3& interSectionPoint)
		: m_RenderComponent(a_RenderComponent), m_IntersectionPoint(interSectionPoint)
    {
    }

	const std::shared_ptr<RayHitData> GetClosestRayHitData(std::vector<std::shared_ptr<RayHitData>> rayHitDatas, glm::vec3 rayOrigin)
	{
		std::shared_ptr<RayHitData> closestHit = NULL;
		float closestDistance = INFINITY;

		for (std::shared_ptr<RayHitData> rayHitData : rayHitDatas)
		{
			float distance = glm::distance(rayOrigin, rayHitData->m_IntersectionPoint);

			if (distance < closestDistance)
			{
				closestHit = rayHitData;
				closestDistance = distance;
			}
		}

		return closestHit;
	}

	const void RemoveRayHitsOutOfDistance(std::vector<std::shared_ptr<RayHitData>>& rayHitDatas, glm::vec3 rayOrigin, float maxDistance)
	{
		for (int i = rayHitDatas.size() - 1; i >= 0; i--)
		{
			float distance = glm::distance(rayOrigin, rayHitDatas[i]->m_IntersectionPoint);

			if (distance > maxDistance)
			{
				rayHitDatas.erase(rayHitDatas.begin() + i);
			}
		}
	}
}