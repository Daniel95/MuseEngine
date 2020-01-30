#pragma once

#include "Camera.h"

namespace Muse
{
	struct Ray;

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(const glm::vec3& position, const glm::vec3& lookAt, int screenSizeX, int screenSizeY, float vfov, const glm::vec3& up = glm::vec3(0, 1, 0));
		~PerspectiveCamera();
		virtual const std::shared_ptr<Ray> GetLookingRay(float pixelX, float pixelY) const override;
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