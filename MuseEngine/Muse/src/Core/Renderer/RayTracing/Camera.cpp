#include "MusePCH.h"

#include "Camera.h"
#include "Core/Gameplay/Component/TransformComponent.h"

namespace Muse
{
	Camera::Camera(const glm::vec3& position, const glm::vec3& lookAt, int screenSizeX, int screenSizeY) 
		: m_TransformComponent(*new TransformComponent()), lookAt(lookAt), direction(glm::normalize(lookAt - position)),
          screenSizeX(screenSizeX), screenSizeY(screenSizeY)
    {
		m_TransformComponent.SetPosition(position);
    }

	void Camera::GetScreenSize(int & xSize, int & ySize) const
	{
		xSize = screenSizeX;
		ySize = screenSizeY;
	}

	void Camera::Move(glm::vec3 movement)
	{
		m_TransformComponent.Move(movement);
	}

	void Camera::MoveLookAt(const glm::vec3 & movement)
	{
		lookAt += movement;
		direction = (lookAt - glm::normalize(m_TransformComponent.GetPosition()));
	}
}