#include "MusePCH.h"

#include "BVH.h"
#include "BoundingVolume.h"
#include "Core/Renderer/RayTracer/Shape/Shape.h"
#include "Core/Renderer/RayTracer/RayHitData.h"
#include "Core/Renderer/RayTracer/Ray.h"
#include "Core/Scene/SceneOld.h"

namespace Muse
{
	BVH::BVH(SceneOld& a_Scene)
		: scene(a_Scene)
	{
	}

	bool BVH::RayCast(std::vector<RayHitData>& a_RayHitDatas, const Ray& a_Ray, float a_MaxDistance) const
	{
		ASSERT_ENGINE(!boundingVolumes.empty(), "No bounding volumes!");

		for (const BoundingVolume* boundingVolume : boundingVolumes)
		{
			CheckBoundingVolume(a_RayHitDatas, *boundingVolume, a_Ray, a_MaxDistance);
		}

		return !a_RayHitDatas.empty();
	}

	void BVH::CheckBoundingVolume(std::vector<RayHitData>& a_RayHitDatas, const BoundingVolume& a_BoundingVolumeToCheck, const Ray& a_Ray, float a_MaxDistance) const
	{
	/*
		scene.IncreaseRaysSend();

		glm::vec3 intersectionPoint;
		if (!a_BoundingVolumeToCheck.boundingVolumeShape.CheckRayHit(intersectionPoint, a_Ray)) { return; }

		scene.RayCast(a_RayHitDatas, a_BoundingVolumeToCheck.m_RenderComponents, a_Ray, a_MaxDistance);

		for (const BoundingVolume* boundingVolume : a_BoundingVolumeToCheck.m_ChildrenBoundingVolumes)
		{
			CheckBoundingVolume(a_RayHitDatas, *boundingVolume, a_Ray, a_MaxDistance);
		}
    */
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

	void BVH::PrintBoundingVolume(const BoundingVolume& a_BoundingVolume, int a_Layer, int& a_Total) const
	{
		PrintLayerSpacing(a_Layer);

		std::cout << "____________  LAYER: " << a_Layer << std::endl;
		PrintLayerSpacing(a_Layer);
		std::cout << "SceneObject Count: " << a_BoundingVolume.m_RenderComponents.size() << std::endl;
		std::cout << std::endl;

		a_Layer++;
		a_Total++;

		for (const BoundingVolume* childBoundingVolume : a_BoundingVolume.m_ChildrenBoundingVolumes)
		{
			PrintBoundingVolume(*childBoundingVolume, a_Layer, a_Total);
		}
	}
}