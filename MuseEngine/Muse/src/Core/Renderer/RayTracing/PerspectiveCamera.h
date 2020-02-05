#pragma once

#include "Camera.h"

namespace Muse
{
	struct Ray;

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(const glm::vec3& a_Position, const glm::vec3& a_LookAt, int screenSizeX, int a_ScreenSizeY, float a_Vfov, const glm::vec3& a_Up = glm::vec3(0, 1, 0));
		virtual void GetLookingRay(Ray& a_Ray, float a_PixelX, float a_PixelY) const override;
		virtual void Move(glm::vec3 movement) override;
		virtual void MoveLookAt(const glm::vec3& movement);

	private:
		void UpdateLookingPlane();

		glm::vec3 horizontal;
		glm::vec3 vertical;
		glm::vec3 lowerLeftCorner;
		glm::vec2 size;
		glm::vec3 up;
		glm::vec3 lookAt;

	};

}