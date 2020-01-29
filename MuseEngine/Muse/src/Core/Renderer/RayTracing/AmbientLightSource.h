#pragma once

#include "LightSource.h"

class AmbientLightSource : public LightSource
{
public:
	AmbientLightSource(const glm::vec4 & color, float intensity);
	virtual ~AmbientLightSource();
	virtual glm::vec4 GetLight(const glm::vec3 & point) const override;

};

