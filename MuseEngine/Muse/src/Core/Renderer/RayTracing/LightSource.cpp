#include "MusePCH.h"

#include "LightSource.h"
#include <algorithm>

LightSource::LightSource(const glm::vec3 & position, const glm::vec3 & color, float intensity, float maxRange)
	: position(position), intensity(intensity), maxRange(maxRange), color(color) { }

LightSource::~LightSource() { }

glm::vec3 LightSource::GetLight(const glm::vec3 & point) const
{
	const float distanceFromLight = distance(position, point);
	const float falloff = 1 - std::clamp((distanceFromLight / maxRange), 0.0f, 1.0f);
	const glm::vec3 light = (color * intensity) * falloff;
	return light;
}

glm::vec3 LightSource::GetDirectionToPoint(const glm::vec3 & point) const
{
	return (point - position).normalized();
}

const glm::vec3 & LightSource::GetPosition() const
{
	return position;
}
