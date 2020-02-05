#include "MusePCH.h"

#include "PerspectiveCamera.h"

#include "Core/Utilities/Defines.h"
#include "Core/Gameplay/Component/TransformComponent.h"

namespace Muse
{
	PerspectiveCamera::PerspectiveCamera(const glm::vec3& a_Position, const glm::vec3& a_LookAt, int screenSizeX, int a_ScreenSizeY, float a_Vfov, const glm::vec3& a_Up)
		: Camera(a_Position, a_LookAt, screenSizeX, a_ScreenSizeY), up(a_Up)
	{
		direction = glm::normalize(a_LookAt - a_Position);

		const float aspect = static_cast<float>(screenSizeX) / static_cast<float>(a_ScreenSizeY);
		const float theta = static_cast<float>(a_Vfov * PI / 180);
		size.y = tan(theta);
		size.x = aspect * size.y;

		UpdateLookingPlane();
	}

	void PerspectiveCamera::GetLookingRay(Ray& ray, float a_PixelX, float a_PixelY) const
	{
		ray.Origin = m_TransformComponent.GetPosition();

		const float x = a_PixelX / static_cast<float>(screenSizeX);
		const float y = a_PixelY / static_cast<float>(screenSizeY);

		const glm::vec3 lookingPlanePoint = lowerLeftCorner + x * horizontal + y * vertical;
		ray.Direction = glm::normalize(lookingPlanePoint - m_TransformComponent.GetPosition());
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
