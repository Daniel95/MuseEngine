#include "MusePCH.h"

#include "RayHitData.h"
#include "Core/Gameplay/Component/RenderComponent.h"

namespace Muse
{
	/*
    RayHitData::RayHitData(std::shared_ptr<const RenderComponent> a_RenderComponent,
		const glm::vec3& a_InterSectionPoint)
		: m_RenderComponent(a_RenderComponent), m_IntersectionPoint(a_InterSectionPoint)
    {
    }
    */

	int RayHitData::GetClosestRayHitDataIndex(const std::vector<RayHitData>& a_RayHitDatas, const glm::vec3& a_RayOrigin)
	{
		int closestRayHitDataIndex = 0;
		float closestDistance = INFINITY;

		for (size_t i = 0; i < a_RayHitDatas.size(); i++)
		{
			float distance = glm::distance(a_RayOrigin, a_RayHitDatas.at(i).m_IntersectionPoint);

			if (distance < closestDistance)
			{
				closestRayHitDataIndex = i;
				closestDistance = distance;
			}
		}

		return closestRayHitDataIndex;
	}

	void RayHitData::RemoveRayHitsOutOfDistance(std::vector<RayHitData>& a_RayHitDatas, const glm::vec3& a_RayOrigin, float a_MaxDistance)
	{
		for (int i = a_RayHitDatas.size() - 1; i >= 0; i--)
		{
			float distance = glm::distance(a_RayOrigin, a_RayHitDatas.at(i).m_IntersectionPoint);

			if (distance > a_MaxDistance)
			{
				a_RayHitDatas.erase(a_RayHitDatas.begin() + i);
			}
		}
	}
}