#pragma once

#include "Camera.h"
#include "vec2.h"
#include "vec3.h"

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(const vec3 & position, const vec3 & direction, int screenSizeX, int screenSizeY, const vec2 & viewPortWorldSize);
	~OrthographicCamera();
	virtual const std::shared_ptr<Ray> GetLookingRay(float pixelX, float pixelY) const override;
	vec3 PixelToWorldPosition(float xPixel, float yPixel) const;

private:
	vec2 viewPortWorldSize;

};

