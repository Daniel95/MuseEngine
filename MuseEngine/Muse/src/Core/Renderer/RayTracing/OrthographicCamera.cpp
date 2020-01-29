#include "MusePCH.h"

#include <iostream>

#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(const vec3 & position, const vec3 & lookAt, int screenSizeX, int screenSizeY, const vec2 & viewPortWorldSize)
	: Camera(position, lookAt, screenSizeX, screenSizeY), viewPortWorldSize(viewPortWorldSize)
{
	direction = (lookAt - position).normalized();
	direction.m_Y *= -1;
}

OrthographicCamera::~OrthographicCamera() { }

const std::shared_ptr<Ray> OrthographicCamera::GetLookingRay(float pixelX, float pixelY) const
{
	std::shared_ptr<Ray> ray = std::make_shared<Ray>();

	ray->Origin = PixelToWorldPosition(pixelX, pixelY);
	ray->Direction = direction;

	return ray;
}

vec3 OrthographicCamera::PixelToWorldPosition(float xPixel, float yPixel) const
{
	const float xProgress = xPixel / static_cast<float>(screenSizeX);
	const float yProgress = yPixel / static_cast<float>(screenSizeY);

	const vec3 halfViewPortSize = vec3(viewPortWorldSize.m_X / 2, viewPortWorldSize.m_Y / 2, 0);
	const vec3 viewPortPosition = vec3(xProgress * viewPortWorldSize.m_X, yProgress * viewPortWorldSize.m_Y, 0) - halfViewPortSize;
	vec3 viewPortWorldPosition = transform.GetPosition() + direction + viewPortPosition;

	viewPortWorldPosition.m_Y *= -1;

	return viewPortWorldPosition;
}
