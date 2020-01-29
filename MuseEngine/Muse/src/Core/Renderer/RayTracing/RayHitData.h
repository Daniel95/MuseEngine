#pragma once

#include <vector>

#include "MathLibrary.h"
#include "vec3.h"

class SceneObject;

struct RayHitData
{
	RayHitData(const SceneObject* hitSceneObject, const vec3 interSectionPoint);

	const SceneObject* HitSceneObject;
	const vec3 IntersectionPoint;
};

const std::shared_ptr<RayHitData> GetClosestRayHitData(std::vector<std::shared_ptr<RayHitData>> rayHitDatas, vec3 rayOrigin);
const void RemoveRayHitsOutOfDistance(std::vector<std::shared_ptr<RayHitData>>& rayHitDatas, vec3 rayOrigin, float maxDistance);