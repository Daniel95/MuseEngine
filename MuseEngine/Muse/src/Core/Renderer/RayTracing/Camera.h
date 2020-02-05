#pragma once

#include "Ray.h"

#include <glm/glm.hpp>

namespace Muse
{
	class TransformComponent;

	class Camera
	{
	public:
		Camera(const glm::vec3 & position, const glm::vec3 & lookAt, int screenSizeX, int screenSizeY);
		virtual ~Camera() = default;
		virtual void GetLookingRay(Ray& a_Ray, float pixelX, float pixelY) const = 0;
		void GetScreenSize(int & xSize, int & ySize) const;
		virtual void Move(glm::vec3 movement);
		virtual void MoveLookAt(const glm::vec3 & movement);

	protected:
		const int screenSizeX;
		const int screenSizeY;
		glm::vec3 direction;
		glm::vec3 lookAt;
		TransformComponent& m_TransformComponent;

	};
}