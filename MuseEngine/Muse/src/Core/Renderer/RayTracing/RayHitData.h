#pragma once

#include <vector>

#include <glm/glm.hpp>

class SceneObject;

struct RayHitData
{
	RayHitData(const SceneObject* hitSceneObject, const glm::vec3 interSectionPoint);

	const SceneObject* HitSceneObject;
	const glm::vec3 IntersectionPoint;
};

const std::shared_ptr<RayHitData> GetClosestRayHitData(std::vector<std::shared_ptr<RayHitData>> rayHitDatas, glm::vec3 rayOrigin);
const void RemoveRayHitsOutOfDistance(std::vector<std::shared_ptr<RayHitData>>& rayHitDatas, glm::vec3 rayOrigin, float maxDistance);