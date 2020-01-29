#include "MusePCH.h"

#include "TopDownBVH.h"
#include "SceneObject.h"
#include "BoundingVolume.h"
#include "Box.h"
#include "Shape.h"

TopDownBVH::TopDownBVH(Scene & scene)
	: BVH(scene) { }

TopDownBVH::~TopDownBVH() { }

void TopDownBVH::ConstructHierarchy(std::vector<SceneObject*> & sceneObjects)
{
	boundingVolumes.clear();

	vec3 minBounds;
	vec3 maxBounds;
	GetWorldMinMaxBounds(minBounds, maxBounds, sceneObjects);
	vec3 range = maxBounds - minBounds;
	vec3 center = (minBounds + maxBounds) / 2;

	Box* box = new Box(center, range);
	BoundingVolume* boundingVolume = new BoundingVolume(*box);
	boundingVolumes.push_back(boundingVolume);

	if (sceneObjects.size() > 2)
	{
		Split(*boundingVolume, sceneObjects);
	}
	else
	{
		for (SceneObject* sceneObject : sceneObjects)
		{
			boundingVolume->childrenSceneObjects.push_back(sceneObject);
		}
	}

}

void TopDownBVH::Split(BoundingVolume & parentBoundingVolume, std::vector<SceneObject*> & sceneObjectsToGroup) const
{
	//Start with placing two bounding volumes inside the parent bounding volume, that split the space evenly.
	vec3 parentBoundingVolumePosition = parentBoundingVolume.boundingVolumeShape.GetTransform().GetPosition();
	vec3 parentBoundingVolumeScale = parentBoundingVolume.boundingVolumeShape.GetTransform().GetScale();

	vec3 box1Position;
	vec3 box2Position;
	vec3 boxScale;

	if (parentBoundingVolumeScale.m_X >= parentBoundingVolumeScale.m_Y && 
		parentBoundingVolumeScale.m_X >= parentBoundingVolumeScale.m_Z)
	{
		boxScale = vec3(parentBoundingVolumeScale.m_X / 2, parentBoundingVolumeScale.m_Y, parentBoundingVolumeScale.m_Z);
		box1Position = vec3(parentBoundingVolumePosition.m_X - parentBoundingVolumeScale.m_X / 4, parentBoundingVolumePosition.m_Y, parentBoundingVolumePosition.m_Z);
		box2Position = vec3(parentBoundingVolumePosition.m_X + parentBoundingVolumeScale.m_X / 4, parentBoundingVolumePosition.m_Y, parentBoundingVolumePosition.m_Z);
	}
	else if (parentBoundingVolumeScale.m_Y >= parentBoundingVolumeScale.m_X && 
		parentBoundingVolumeScale.m_Y >= parentBoundingVolumeScale.m_Z)
	{
		boxScale = vec3(parentBoundingVolumeScale.m_X, parentBoundingVolumeScale.m_Y / 2, parentBoundingVolumeScale.m_Z);
		box1Position = vec3(parentBoundingVolumePosition.m_X, parentBoundingVolumePosition.m_Y - parentBoundingVolumeScale.m_Y / 4, parentBoundingVolumePosition.m_Z);
		box2Position = vec3(parentBoundingVolumePosition.m_X, parentBoundingVolumePosition.m_Y + parentBoundingVolumeScale.m_Y / 4, parentBoundingVolumePosition.m_Z);
	}
	else
	{
		boxScale = vec3(parentBoundingVolumeScale.m_X, parentBoundingVolumeScale.m_Y, parentBoundingVolumeScale.m_Z / 2);
		box1Position = vec3(parentBoundingVolumePosition.m_X, parentBoundingVolumePosition.m_Y, parentBoundingVolumePosition.m_Z - parentBoundingVolumeScale.m_Z / 4);
		box2Position = vec3(parentBoundingVolumePosition.m_X, parentBoundingVolumePosition.m_Y, parentBoundingVolumePosition.m_Z + parentBoundingVolumeScale.m_Z / 4);
	}

	//Sort the sceneObjects over the two new boundubg volumes box spaces, and the parent boundubg volume.
	std::vector<SceneObject*> sceneObjectsWithinbox1;
	std::vector<SceneObject*> sceneObjectsWithinbox2;

	vec3 halfBoxScale = boxScale / 2;
	vec3 box1MinBounds = box1Position - halfBoxScale;
	vec3 box1MaxBounds = box1Position + halfBoxScale;

	vec3 box2MinBounds = box2Position - halfBoxScale;
	vec3 box2MaxBounds = box2Position + halfBoxScale;

	for (SceneObject* sceneObject : sceneObjectsToGroup)
	{
		bool overlapWithBoundingVolume1 = CheckShapeOverlap(sceneObject->GetShape(), box1MinBounds, box1MaxBounds);
		bool overlapWithBoundingVolume2 = CheckShapeOverlap(sceneObject->GetShape(), box2MinBounds, box2MaxBounds);

		assert(overlapWithBoundingVolume1 || overlapWithBoundingVolume2);

		if (overlapWithBoundingVolume1 && overlapWithBoundingVolume2)
		{
			parentBoundingVolume.childrenSceneObjects.push_back(sceneObject);
		}
		else
		{
			if (overlapWithBoundingVolume1)
			{
				sceneObjectsWithinbox1.push_back(sceneObject);
			}
			else
			{
				sceneObjectsWithinbox2.push_back(sceneObject);
			}
		}
	}

	//Check if the new boundings boxes should be created, and if so create them.
	//If the bounding boxes have more then one SceneObject as child, split them as well.
	if (sceneObjectsWithinbox1.size() > 2)
	{
		BoundingVolume& newBoundingVolume = *new BoundingVolume(*new Box(box1Position, boxScale));
		parentBoundingVolume.childrenBoundingVolumes.push_back(&newBoundingVolume);
		Split(newBoundingVolume, sceneObjectsWithinbox1);
	}
	else 
	{
		for (SceneObject* sceneObject : sceneObjectsWithinbox1)
		{
			parentBoundingVolume.childrenSceneObjects.push_back(sceneObject);
		}
	}

	if (sceneObjectsWithinbox2.size() > 2)
	{
		BoundingVolume& newBoundingVolume = *new BoundingVolume(*new Box(box2Position, boxScale));
		parentBoundingVolume.childrenBoundingVolumes.push_back(&newBoundingVolume);
		Split(newBoundingVolume, sceneObjectsWithinbox2);
	}
	else
	{
		for (SceneObject* sceneObject : sceneObjectsWithinbox2)
		{
			parentBoundingVolume.childrenSceneObjects.push_back(sceneObject);
		}
	}

}

bool TopDownBVH::CheckShapeOverlap(const Shape & shape, const vec3 & minBound, const vec3 & maxBound) const
{
	vec3 shapeMinBound;
	vec3 shapeMaxBound;

	shape.GetMinMaxBounds(shapeMinBound, shapeMaxBound);

	return (shapeMaxBound.m_X >= minBound.m_X && maxBound.m_X >= shapeMinBound.m_X) &&
		(shapeMaxBound.m_Y >= minBound.m_Y && maxBound.m_Y >= shapeMinBound.m_Y) &&
		(shapeMaxBound.m_Z >= minBound.m_Z && maxBound.m_Z >= shapeMinBound.m_Z);
}

void TopDownBVH::GetWorldMinMaxBounds(vec3 & minBound, vec3 & maxBound, const std::vector<SceneObject*> & sceneObjects) const
{
	vec3 sceneObjectMinBound;
	vec3 sceneObjectMaxBound;

	minBound = vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	maxBound = vec3(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

	for (SceneObject* sceneObject : sceneObjects)
	{
		sceneObject->GetShape().GetMinMaxBounds(sceneObjectMinBound, sceneObjectMaxBound);

		minBound.m_X = std::min(sceneObjectMinBound.m_X, minBound.m_X);
		minBound.m_Y = std::min(sceneObjectMinBound.m_Y, minBound.m_Y);
		minBound.m_Z = std::min(sceneObjectMinBound.m_Z, minBound.m_Z);

		maxBound.m_X = std::max(sceneObjectMaxBound.m_X, maxBound.m_X);
		maxBound.m_Y = std::max(sceneObjectMaxBound.m_Y, maxBound.m_Y);
		maxBound.m_Z = std::max(sceneObjectMaxBound.m_Z, maxBound.m_Z);
	}
}
