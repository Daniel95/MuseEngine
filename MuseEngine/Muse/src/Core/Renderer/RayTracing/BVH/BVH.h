#pragma once

#include <vector>
#include <memory>


namespace Muse
{

	class BoundingVolume;
	class Scene;
	struct Ray;
	struct RayHitData;
	class GameObject;

	class BVH
	{
	public:
		BVH(Scene& a_Scene);
		virtual ~BVH() = default;

		virtual void ConstructHierarchy(const std::vector<std::shared_ptr<GameObject>>& a_GameObject) = 0;
		bool RayCast(const std::vector<std::shared_ptr<RayHitData>>& rayHitDatas, std::shared_ptr<Ray> ray, float maxDistance = INFINITY) const;
		void CheckBoundingVolume(const std::vector<std::shared_ptr<RayHitData>>& rayHitDatas, const BoundingVolume& boundingVolumeToCheck, const std::shared_ptr<Ray> ray, float maxDistance = INFINITY)  const;
		void PrintHierarchy() const;
		void PrintLayerSpacing(int layer) const;
		void PrintBoundingVolume(const BoundingVolume& boundingVolume, int layer, int& total) const;

	protected:
		Scene& scene;
		std::vector<BoundingVolume*> boundingVolumes;

	};

}