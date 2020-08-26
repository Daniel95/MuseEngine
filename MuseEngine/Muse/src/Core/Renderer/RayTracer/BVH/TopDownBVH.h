#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "BVH.h"

namespace Muse
{
    class RenderComponent;
    class GameObject;
	class Shape;

	class TopDownBVH
		: public BVH
	{
	public:
		TopDownBVH(SceneOld& scenes);
		virtual ~TopDownBVH();

		virtual void ConstructHierarchy(const std::vector<std::shared_ptr<RenderComponent>>& a_RenderComponents) override;
		void Split(BoundingVolume& parentBoundingVolume, const std::vector<std::shared_ptr<RenderComponent>>& a_RenderComponents) const;
        static bool CheckShapeOverlap(std::shared_ptr<const Shape> shape, const glm::vec3& minBound, const glm::vec3& maxBound);
		void GetWorldMinMaxBounds(glm::vec3& minBound, glm::vec3& maxBound, const std::vector<std::shared_ptr<RenderComponent>>& a_RenderComponents) const;

	};
}