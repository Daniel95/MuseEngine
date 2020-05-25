#include "MusePCH.h"

#include "BoundingVolume.h"

namespace Muse
{
    BoundingVolume::BoundingVolume(Shape& boundingVolumeShape)
	    : boundingVolumeShape(boundingVolumeShape)
    { }

    BoundingVolume::~BoundingVolume() { }
}