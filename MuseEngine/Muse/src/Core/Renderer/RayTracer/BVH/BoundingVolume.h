#pragma once

#include <vector>

namespace Muse
{
    class RenderComponent;
    class Shape;
	class GameObject;

	class BoundingVolume
	{
	public:
		BoundingVolume(Shape& boundingVolumeShape);
		virtual ~BoundingVolume();

		Shape& boundingVolumeShape;
		std::vector<BoundingVolume*> m_ChildrenBoundingVolumes;
		std::vector<std::shared_ptr<RenderComponent>> m_RenderComponents;

	};

}