#include "MusePCH.h"

#include "TopDownBVH.h"
#include "BoundingVolume.h"
#include "Core/Renderer/RayTracer/Shape/Box.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/RenderComponent.h"

namespace Muse
{
	TopDownBVH::TopDownBVH(Scene& scene)
		: BVH(scene)
	{
	}

	TopDownBVH::~TopDownBVH() {}

	void TopDownBVH::ConstructHierarchy(const std::vector<std::shared_ptr<RenderComponent>>& a_RenderComponents)
	{
		boundingVolumes.clear();

		glm::vec3 minBounds;
		glm::vec3 maxBounds;
		GetWorldMinMaxBounds(minBounds, maxBounds, a_RenderComponents);
		glm::vec3 range = maxBounds - minBounds;
		glm::vec3 center = (minBounds + maxBounds) / 2.f;

		//Box* box = new Box(center, range);
		Box* box = new Box();
		BoundingVolume* boundingVolume = new BoundingVolume(*box);
		boundingVolumes.push_back(boundingVolume);

		if (a_RenderComponents.size() > 2)
		{
			Split(*boundingVolume, a_RenderComponents);
		}
		else
		{
			for (std::shared_ptr<RenderComponent> sceneObject : a_RenderComponents)
			{
				boundingVolume->m_RenderComponents.push_back(sceneObject);
			}
		}

	}

	void TopDownBVH::Split(BoundingVolume& parentBoundingVolume, const std::vector<std::shared_ptr<RenderComponent>>& a_RenderComponents) const
	{
		//Start with placing two bounding volumes inside the parent bounding volume, that split the space evenly.
		glm::vec3 parentBoundingVolumePosition = parentBoundingVolume.boundingVolumeShape.GetTransform()->GetLocalPosition();
		glm::vec3 parentBoundingVolumeScale = parentBoundingVolume.boundingVolumeShape.GetTransform()->GetLocalScale();

		glm::vec3 box1Position;
		glm::vec3 box2Position;
		glm::vec3 boxScale;

		if (parentBoundingVolumeScale.x >= parentBoundingVolumeScale.y &&
			parentBoundingVolumeScale.x >= parentBoundingVolumeScale.z)
		{
			boxScale = glm::vec3(parentBoundingVolumeScale.x / 2, parentBoundingVolumeScale.y, parentBoundingVolumeScale.z);
			box1Position = glm::vec3(parentBoundingVolumePosition.x - parentBoundingVolumeScale.x / 4, parentBoundingVolumePosition.y, parentBoundingVolumePosition.z);
			box2Position = glm::vec3(parentBoundingVolumePosition.x + parentBoundingVolumeScale.x / 4, parentBoundingVolumePosition.y, parentBoundingVolumePosition.z);
		}
		else if (parentBoundingVolumeScale.y >= parentBoundingVolumeScale.x &&
			parentBoundingVolumeScale.y >= parentBoundingVolumeScale.z)
		{
			boxScale = glm::vec3(parentBoundingVolumeScale.x, parentBoundingVolumeScale.y / 2, parentBoundingVolumeScale.z);
			box1Position = glm::vec3(parentBoundingVolumePosition.x, parentBoundingVolumePosition.y - parentBoundingVolumeScale.y / 4, parentBoundingVolumePosition.z);
			box2Position = glm::vec3(parentBoundingVolumePosition.x, parentBoundingVolumePosition.y + parentBoundingVolumeScale.y / 4, parentBoundingVolumePosition.z);
		}
		else
		{
			boxScale = glm::vec3(parentBoundingVolumeScale.x, parentBoundingVolumeScale.y, parentBoundingVolumeScale.z / 2);
			box1Position = glm::vec3(parentBoundingVolumePosition.x, parentBoundingVolumePosition.y, parentBoundingVolumePosition.z - parentBoundingVolumeScale.z / 4);
			box2Position = glm::vec3(parentBoundingVolumePosition.x, parentBoundingVolumePosition.y, parentBoundingVolumePosition.z + parentBoundingVolumeScale.z / 4);
		}

		//Sort the sceneObjects over the two new boundubg volumes box spaces, and the parent boundubg volume.
		std::vector<std::shared_ptr<RenderComponent>> sceneObjectsWithinbox1;
		std::vector<std::shared_ptr<RenderComponent>> sceneObjectsWithinbox2;

		glm::vec3 halfBoxScale = boxScale / 2.f;
		glm::vec3 box1MinBounds = box1Position - halfBoxScale;
		glm::vec3 box1MaxBounds = box1Position + halfBoxScale;

		glm::vec3 box2MinBounds = box2Position - halfBoxScale;
		glm::vec3 box2MaxBounds = box2Position + halfBoxScale;

		for (std::shared_ptr<RenderComponent> renderComponent : a_RenderComponents)
		{
			std::shared_ptr<const Shape> shape = renderComponent->GetShape();

			bool overlapWithBoundingVolume1 = CheckShapeOverlap(shape, box1MinBounds, box1MaxBounds);
			bool overlapWithBoundingVolume2 = CheckShapeOverlap(shape, box2MinBounds, box2MaxBounds);

			assert(overlapWithBoundingVolume1 || overlapWithBoundingVolume2);

			if (overlapWithBoundingVolume1 && overlapWithBoundingVolume2)
			{
				parentBoundingVolume.m_RenderComponents.push_back(renderComponent);
			}
			else
			{
				if (overlapWithBoundingVolume1)
				{
					sceneObjectsWithinbox1.push_back(renderComponent);
				}
				else
				{
					sceneObjectsWithinbox2.push_back(renderComponent);
				}
			}
		}

		//Check if the new boundings boxes should be created, and if so create them.
		//If the bounding boxes have more then one SceneObject as child, split them as well.
		if (sceneObjectsWithinbox1.size() > 2)
		{
			//BoundingVolume& newBoundingVolume = *new BoundingVolume(*new Box(box1Position, boxScale));
			BoundingVolume& newBoundingVolume = *new BoundingVolume(*new Box());
			parentBoundingVolume.m_ChildrenBoundingVolumes.push_back(&newBoundingVolume);
			Split(newBoundingVolume, sceneObjectsWithinbox1);
		}
		else
		{
			for (std::shared_ptr<RenderComponent> sceneObject : sceneObjectsWithinbox1)
			{
				parentBoundingVolume.m_RenderComponents.push_back(sceneObject);
			}
		}

		if (sceneObjectsWithinbox2.size() > 2)
		{
			BoundingVolume& newBoundingVolume = *new BoundingVolume(*new Box());
			parentBoundingVolume.m_ChildrenBoundingVolumes.push_back(&newBoundingVolume);
			Split(newBoundingVolume, sceneObjectsWithinbox2);
		}
		else
		{
			for (std::shared_ptr<RenderComponent> sceneObject : sceneObjectsWithinbox2)
			{
				parentBoundingVolume.m_RenderComponents.push_back(sceneObject);
			}
		}
	}

	bool TopDownBVH::CheckShapeOverlap(std::shared_ptr<const Shape> shape, const glm::vec3& minBound, const glm::vec3& maxBound)
    {
		glm::vec3 shapeMinBound;
		glm::vec3 shapeMaxBound;

		shape->GetMinMaxBounds(shapeMinBound, shapeMaxBound);

		return (shapeMaxBound.x >= minBound.x && maxBound.x >= shapeMinBound.x) &&
			(shapeMaxBound.y >= minBound.y && maxBound.y >= shapeMinBound.y) &&
			(shapeMaxBound.z >= minBound.z && maxBound.z >= shapeMinBound.z);
	}

	void TopDownBVH::GetWorldMinMaxBounds(glm::vec3& minBound, glm::vec3& maxBound, const std::vector<std::shared_ptr<RenderComponent>>& a_RenderComponents) const
	{
		glm::vec3 sceneObjectMinBound;
		glm::vec3 sceneObjectMaxBound;

		minBound = glm::vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maxBound = glm::vec3(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

		for (std::shared_ptr<RenderComponent> sceneObject : a_RenderComponents)
		{
			std::shared_ptr<const Shape> shape = sceneObject->GetGameObject()->GetComponent<RenderComponent>()->GetShape();

			shape->GetMinMaxBounds(sceneObjectMinBound, sceneObjectMaxBound);

			minBound.x = std::min(sceneObjectMinBound.x, minBound.x);
			minBound.y = std::min(sceneObjectMinBound.y, minBound.y);
			minBound.z = std::min(sceneObjectMinBound.z, minBound.z);

			maxBound.x = std::max(sceneObjectMaxBound.x, maxBound.x);
			maxBound.y = std::max(sceneObjectMaxBound.y, maxBound.y);
			maxBound.z = std::max(sceneObjectMaxBound.z, maxBound.z);
		}
	}
}
