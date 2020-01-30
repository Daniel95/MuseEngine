#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "BVH.h"

namespace Muse
{
	class GameObject;
	class Shape;

	class TopDownBVH
		: public BVH
	{
	public:
		TopDownBVH(Scene& scenes);
		virtual ~TopDownBVH();

		virtual void ConstructHierarchy(const std::vector<GameObject*>& a_GameObjects) override;
		void Split(BoundingVolume& parentBoundingVolume, const std::vector<GameObject*>& sceneObjectsLeftToGroup) const;
		bool CheckShapeOverlap(const Shape& shape, const glm::vec3& minBound, const glm::vec3& maxBound) const;
		void GetWorldMinMaxBounds(glm::vec3& minBound, glm::vec3& maxBound, const std::vector<GameObject*>& sceneObjects) const;

	};
}