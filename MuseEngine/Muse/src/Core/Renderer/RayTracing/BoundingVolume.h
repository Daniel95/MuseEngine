#pragma once

#include <vector>

class Shape;
class SceneObject;

class BoundingVolume
{
public:
	BoundingVolume(Shape& boundingVolumeShape);
	virtual ~BoundingVolume();

	Shape& boundingVolumeShape;
	std::vector<BoundingVolume*> childrenBoundingVolumes;
	std::vector<SceneObject*> childrenSceneObjects;

};

