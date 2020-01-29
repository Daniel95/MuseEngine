#include "MusePCH.h"

#include "BoundingVolume.h"

BoundingVolume::BoundingVolume(Shape& boundingVolumeShape)
	: boundingVolumeShape(boundingVolumeShape)
{ }

BoundingVolume::~BoundingVolume() { }
