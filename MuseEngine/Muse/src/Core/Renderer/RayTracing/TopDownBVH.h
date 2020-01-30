#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "BVH.h"

class SceneObject;
class Shape;

class TopDownBVH
	: public BVH
{
public:
	TopDownBVH(Scene & scenes);
	virtual ~TopDownBVH();

	virtual void ConstructHierarchy(std::vector<SceneObject*> & sceneObjects) override;
	void Split(BoundingVolume & parentBoundingVolume, std::vector<SceneObject*> & sceneObjectsLeftToGroup) const;
	bool CheckShapeOverlap(const Shape & shape, const glm::vec3 & minBound, const glm::vec3 & maxBound) const;
	void GetWorldMinMaxBounds(glm::vec3 & minBound, glm::vec3 & maxBound, const std::vector<SceneObject*> & sceneObjects) const;

};

