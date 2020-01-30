#pragma once

#include <vector>

namespace Muse
{
	class Shape;
	class GameObject;

	class BoundingVolume
	{
	public:
		BoundingVolume(Shape& boundingVolumeShape);
		virtual ~BoundingVolume();

		Shape& boundingVolumeShape;
		std::vector<BoundingVolume*> childrenBoundingVolumes;
		std::vector<GameObject*> childrenSceneObjects;

	};

}