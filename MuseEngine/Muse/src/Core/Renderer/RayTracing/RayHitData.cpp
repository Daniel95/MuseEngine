#include "MusePCH.h"

#include "RayHitData.h"
#include "Shape.h"

RayHitData::RayHitData(const SceneObject* hitSceneObject, const glm::vec3 interSectionPoint)
	: HitSceneObject(hitSceneObject), IntersectionPoint(interSectionPoint) { }

const std::shared_ptr<RayHitData> GetClosestRayHitData(std::vector<std::shared_ptr<RayHitData>> rayHitDatas, glm::vec3 rayOrigin)
{
	std::shared_ptr<RayHitData> closestHit = NULL;
	float closestDistance = INFINITY;

	for (std::shared_ptr<RayHitData> rayHitData : rayHitDatas)
	{
		float distance = rayOrigin.distance(rayHitData->IntersectionPoint);

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
		float distance = rayOrigin.distance(rayHitDatas[i]->IntersectionPoint);

		if (distance > maxDistance)
		{
			rayHitDatas.erase(rayHitDatas.begin() + i);
		}
	}
}
