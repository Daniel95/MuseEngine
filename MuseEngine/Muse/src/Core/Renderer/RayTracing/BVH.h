#pragma once

#include <vector>
#include <memory>

class BoundingVolume;
class Scene;
struct Ray;
struct RayHitData;
class SceneObject;

class BVH
{
public:
	BVH(Scene & scene);
	~BVH();

	virtual void ConstructHierarchy(std::vector<SceneObject*> & sceneObject) = 0;
	bool RayCast(std::vector<std::shared_ptr<RayHitData>> & rayHitDatas, const std::shared_ptr<Ray> ray, float maxDistance = INFINITY) const;
	void CheckBoundingVolume(std::vector<std::shared_ptr<RayHitData>> & rayHitDatas, const BoundingVolume & boundingVolumeToCheck, const std::shared_ptr<Ray> ray, float maxDistance = INFINITY)  const;
	void PrintHierarchy() const;
	void PrintLayerSpacing(int layer) const;
	void PrintBoundingVolume(const BoundingVolume & boundingVolume, int layer, int & total) const;

protected:
	Scene & scene;
	std::vector<BoundingVolume*> boundingVolumes;

};

