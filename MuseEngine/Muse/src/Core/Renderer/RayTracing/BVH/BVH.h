#pragma once

#include <vector>
#include <memory>


namespace Muse
{
    class RenderComponent;

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

		virtual void ConstructHierarchy(const std::vector<std::shared_ptr<RenderComponent>>& a_RenderComponents) = 0;
		bool RayCast(const std::vector<std::shared_ptr<RayHitData>>& a_RayHitDatas, std::shared_ptr<Ray> a_Ray, float a_MaxDistance = INFINITY) const;
		void CheckBoundingVolume(const std::vector<std::shared_ptr<RayHitData>>& a_RayHitDatas, const BoundingVolume& a_BoundingVolumeToCheck, std::shared_ptr<Ray> a_Ray, float a_MaxDistance = INFINITY)  const;
		void PrintHierarchy() const;
		void PrintLayerSpacing(int layer) const;
		void PrintBoundingVolume(const BoundingVolume& a_BoundingVolume, int a_Layer, int& a_Total) const;

	protected:
		Scene& scene;
		std::vector<BoundingVolume*> boundingVolumes;

	};

}