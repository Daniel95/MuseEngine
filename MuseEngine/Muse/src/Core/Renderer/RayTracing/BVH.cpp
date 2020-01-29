#include "MusePCH.h"

#include "BVH.h"
#include "Scene.h"
#include "BoundingVolume.h"
#include "SceneObject.h"
#include "Shape.h"
#include "RayHitData.h"
#include "Ray.h"

BVH::BVH(Scene & scene) 
	: scene(scene) { }

BVH::~BVH() { }

bool BVH::RayCast(std::vector<std::shared_ptr<RayHitData>> & rayHitDatas, const std::shared_ptr<Ray> ray, float maxDistance) const
{
	assert(boundingVolumes.size() > 0);

	for (const BoundingVolume* boundingVolume : boundingVolumes)
	{
		CheckBoundingVolume(rayHitDatas, *boundingVolume, ray, maxDistance);
	}

	return rayHitDatas.size() != 0;
}

void BVH::CheckBoundingVolume(std::vector<std::shared_ptr<RayHitData>> & rayHitDatas, const BoundingVolume & boundingVolumeToCheck, const std::shared_ptr<Ray> ray, float maxDistance) const
{
	scene.IncreaseRayCastsSendThisUpdate();

	glm::vec3 intersectionPoint;
	if (!boundingVolumeToCheck.boundingVolumeShape.CheckRayHit(intersectionPoint, ray)) { return; }

	scene.RayCast(rayHitDatas, boundingVolumeToCheck.childrenSceneObjects, ray, maxDistance);

	for (const BoundingVolume* boundingVolume : boundingVolumeToCheck.childrenBoundingVolumes)
	{
		CheckBoundingVolume(rayHitDatas, *boundingVolume, ray, maxDistance);
	}
}

void BVH::PrintHierarchy() const
{
	std::cout << "BVH HIERARCHY:" << std::endl;

	int layer = 0;
	int total = 0;

	for (BoundingVolume* boundingVolume : boundingVolumes)
	{
		PrintBoundingVolume(*boundingVolume, layer, total);
	}

	std::cout << "totalBoundingVolumes: " << total << std::endl;
}

void BVH::PrintLayerSpacing(int layer) const
{
	for (int i = 0; i < layer; i++)
	{
		std::cout << "	";
	}
}

void BVH::PrintBoundingVolume(const BoundingVolume & boundingVolume, int layer, int & total) const
{
	PrintLayerSpacing(layer);

	std::cout << "____________  LAYER: " << layer << std::endl;
	PrintLayerSpacing(layer);
	std::cout << "SceneObject Count: " << boundingVolume.childrenSceneObjects.size() << std::endl;
	std::cout << std::endl;

	layer++;
	total++;

	for (const BoundingVolume* childBoundingVolume : boundingVolume.childrenBoundingVolumes)
	{
		PrintBoundingVolume(*childBoundingVolume, layer, total);
	}
}
