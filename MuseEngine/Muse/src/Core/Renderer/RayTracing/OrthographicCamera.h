#pragma once

#include "Camera.h"

#include <glm/glm.hpp>

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(const glm::vec3 & position, const glm::vec3 & direction, int screenSizeX, int screenSizeY, const glm::vec2 & viewPortWorldSize);
	~OrthographicCamera();
	virtual const std::shared_ptr<Ray> GetLookingRay(float pixelX, float pixelY) const override;
    glm::vec3 PixelToWorldPosition(float xPixel, float yPixel) const;

private:
    glm::vec2 viewPortWorldSize;

};

