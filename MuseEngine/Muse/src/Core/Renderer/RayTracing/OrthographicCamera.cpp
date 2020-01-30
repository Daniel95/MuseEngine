#include "MusePCH.h"

#include <iostream>

#include "OrthographicCamera.h"
#include "Core/Gameplay/Component/TransformComponent.h"

namespace Muse
{

	OrthographicCamera::OrthographicCamera(const glm::vec3& position, const glm::vec3& lookAt, int screenSizeX, int screenSizeY, const glm::vec2& viewPortWorldSize)
		: Camera(position, lookAt, screenSizeX, screenSizeY), viewPortWorldSize(viewPortWorldSize)
	{
		direction = glm::normalize(lookAt - position);
		direction.y *= -1;
	}

	OrthographicCamera::~OrthographicCamera() {}

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

		const glm::vec3 halfViewPortSize = glm::vec3(viewPortWorldSize.x / 2, viewPortWorldSize.y / 2, 0);
		const glm::vec3 viewPortPosition = glm::vec3(xProgress * viewPortWorldSize.x, yProgress * viewPortWorldSize.y, 0) - halfViewPortSize;
		glm::vec3 viewPortWorldPosition = m_TransformComponent.GetPosition() + direction + viewPortPosition;

		viewPortWorldPosition.y *= -1;

		return viewPortWorldPosition;
	}
}
