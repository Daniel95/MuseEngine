#pragma once

#include <vector>

#include "BVH.h"

class SceneObject;
class Shape;
class vec3;
class vec3;

class TopDownBVH
	: public BVH
{
public:
	TopDownBVH(Scene & scenes);
	virtual ~TopDownBVH();

	virtual void ConstructHierarchy(std::vector<SceneObject*> & sceneObjects) override;
	void Split(BoundingVolume & parentBoundingVolume, std::vector<SceneObject*> & sceneObjectsLeftToGroup) const;
	bool CheckShapeOverlap(const Shape & shape, const vec3 & minBound, const vec3 & maxBound) const;
	void GetWorldMinMaxBounds(vec3 & minBound, vec3 & maxBound, const std::vector<SceneObject*> & sceneObjects) const;

};

