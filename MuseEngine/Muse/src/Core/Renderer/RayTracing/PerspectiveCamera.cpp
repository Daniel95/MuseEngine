#include "MusePCH.h"

#include "PerspectiveCamera.h"

#include "Core/Utilities/Defines.h"
#include "Core/Gameplay/Component/TransformComponent.h"

namespace Muse
{
	PerspectiveCamera::PerspectiveCamera(const glm::vec3& position, const glm::vec3& lookAt, int screenSizeX, int screenSizeY, float vfov, const glm::vec3& up)
		: Camera(position, lookAt, screenSizeX, screenSizeY), up(up)
	{
		direction = glm::normalize(lookAt - position);

		const float aspect = static_cast<float>(screenSizeX) / static_cast<float>(screenSizeY);
		const float theta = static_cast<float>(vfov * PI / 180);
		size.y = tan(theta);
		size.x = aspect * size.y;

		UpdateLookingPlane();
	}

	PerspectiveCamera::~PerspectiveCamera() {}

	const std::shared_ptr<Ray> PerspectiveCamera::GetLookingRay(float pixelX, float pixelY) const
	{
		std::shared_ptr<Ray> ray = std::make_shared<Ray>();

		ray->Origin = m_TransformComponent.GetPosition();

		const float invertedPixelY = abs(pixelY - static_cast<float>(screenSizeY));

		const float x = pixelX / static_cast<float>(screenSizeX);
		const float y = invertedPixelY / static_cast<float>(screenSizeY);

		const glm::vec3 lookingPlanePoint = lowerLeftCorner + x * horizontal + y * vertical;
		ray->Direction = glm::normalize(lookingPlanePoint - m_TransformComponent.GetPosition());

		return ray;
	}

	void PerspectiveCamera::Move(glm::vec3 movement)
	{
		movement.x *= -1;

		Camera::Move(movement);
		UpdateLookingPlane();
	}

	void PerspectiveCamera::MoveLookAt(const glm::vec3& movement)
	{
		lookAt += movement;
		direction = glm::normalize(lookAt - m_TransformComponent.GetPosition());

		UpdateLookingPlane();
	}

	void PerspectiveCamera::UpdateLookingPlane()
	{
		const glm::vec3 rightDirection = glm::normalize(glm::cross(up, direction));
		const glm::vec3 upDirection = glm::cross(direction, rightDirection);
		horizontal = size.x * rightDirection;
		vertical = size.y * upDirection;
		lowerLeftCorner = m_TransformComponent.GetPosition() - horizontal * 0.5f - vertical * 0.5f + direction;
	}
}
