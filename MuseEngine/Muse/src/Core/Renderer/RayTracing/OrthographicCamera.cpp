#include "MusePCH.h"

#include <iostream>

#include "OrthographicCamera.h"
#include "Core/Gameplay/Component/TransformComponent.h"

namespace Muse
{

	OrthographicCamera::OrthographicCamera(const glm::vec3& position, const glm::vec3& lookAt, int screenSizeX, int screenSizeY, const glm::vec2& viewPortWorldSize)
		: Camera(position, lookAt, screenSizeX, screenSizeY), m_ViewPortWorldSize(viewPortWorldSize)
	{
		direction = glm::normalize(lookAt - position);
		direction.y *= -1;
	}

	OrthographicCamera::~OrthographicCamera() {}

	void OrthographicCamera::GetLookingRay(Ray& a_Ray, float pixelX, float pixelY) const
	{
		a_Ray.Origin = PixelToWorldPosition(pixelX, pixelY);
		a_Ray.Direction = direction;
	}

	glm::vec3 OrthographicCamera::PixelToWorldPosition(float xPixel, float yPixel) const
	{
		const float xProgress = xPixel / static_cast<float>(screenSizeX);
		const float yProgress = yPixel / static_cast<float>(screenSizeY);

		const glm::vec3 halfViewPortSize = glm::vec3(m_ViewPortWorldSize.x / 2, m_ViewPortWorldSize.y / 2, 0);
		const glm::vec3 viewPortPosition = glm::vec3(xProgress * m_ViewPortWorldSize.x, yProgress * m_ViewPortWorldSize.y, 0) - halfViewPortSize;
		glm::vec3 viewPortWorldPosition = m_TransformComponent.GetPosition() + direction + viewPortPosition;

		viewPortWorldPosition.y *= -1;

		return viewPortWorldPosition;
	}
}
