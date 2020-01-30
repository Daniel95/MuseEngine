#include "MusePCH.h"

#include <iostream>

#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(const glm::vec3 & position, const glm::vec3 & lookAt, int screenSizeX, int screenSizeY, const glm::vec2 & viewPortWorldSize)
	: Camera(position, lookAt, screenSizeX, screenSizeY), viewPortWorldSize(viewPortWorldSize)
{
	direction = (lookAt - position).normalized();
	direction.y *= -1;
}

OrthographicCamera::~OrthographicCamera() { }

const std::shared_ptr<Ray> OrthographicCamera::GetLookingRay(float pixelX, float pixelY) const
{
	std::shared_ptr<Ray> ray = std::make_shared<Ray>();

	ray->Origin = PixelToWorldPosition(pixelX, pixelY);
	ray->Direction = direction;

	return ray;
}

glm::vec3 OrthographicCamera::PixelToWorldPosition(float xPixel, float yPixel) const
{
	const float xProgress = xPixel / static_cast<float>(screenSizeX);
	const float yProgress = yPixel / static_cast<float>(screenSizeY);

	const glm::vec3 halfViewPortSize = glm::vec3(viewPortWorldSize.m_X / 2, viewPortWorldSize.m_Y / 2, 0);
	const glm::vec3 viewPortPosition = glm::vec3(xProgress * viewPortWorldSize.m_X, yProgress * viewPortWorldSize.m_Y, 0) - halfViewPortSize;
    glm::vec3 viewPortWorldPosition = transform.GetPosition() + direction + viewPortPosition;

	viewPortWorldPosition.y *= -1;

	return viewPortWorldPosition;
}
