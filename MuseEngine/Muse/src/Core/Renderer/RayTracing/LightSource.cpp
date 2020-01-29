#include "MusePCH.h"

#include "LightSource.h"
#include <algorithm>

LightSource::LightSource(const vec3 & position, const sf::Color & color, float intensity, float maxRange)
	: position(position), intensity(intensity), maxRange(maxRange), color(color) { }

LightSource::~LightSource() { }

sf::Color LightSource::GetLight(const vec3 & point) const
{
	const float distanceFromLight = distance(position, point);
	const float falloff = 1 - std::clamp((distanceFromLight / maxRange), 0.0f, 1.0f);
	const sf::Color light = (color * intensity) * falloff;
	return light;
}

vec3 LightSource::GetDirectionToPoint(const vec3 & point) const
{
	return (point - position).normalized();
}

const vec3 & LightSource::GetPosition() const
{
	return position;
}
