#pragma once

#include <glm/glm.hpp>

#include "Materialhelper.h"

class LightSource
{
public:
	LightSource(const glm::vec3 & position, const glm::vec4& color, float intensity, float maxRange);
	virtual ~LightSource();
	virtual glm::vec4 GetLight(const glm::vec3 & point) const;
	virtual glm::vec3 GetDirectionToPoint(const glm::vec3 & point) const;
	virtual const glm::vec3 & GetPosition() const;

protected:
	glm::vec3 position;
	float intensity;
	float maxRange;
	glm::vec4 color;

};

