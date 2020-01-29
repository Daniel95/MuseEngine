#include "MusePCH.h"

#include "MaterialHelper.h"
#include "AmbientLightSource.h"

AmbientLightSource::AmbientLightSource(const glm::vec4 & color, float intensity)
	: LightSource(glm::vec3(), color, intensity, INFINITY) { }

AmbientLightSource::~AmbientLightSource() { }

glm::vec4 AmbientLightSource::GetLight(const glm::vec3 & point) const
{
	point;
	return color * intensity;
}