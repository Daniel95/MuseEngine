#pragma once

#include "Transform.h"
#include "Ray.h"

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(const glm::vec3 & position, const glm::vec3 & lookAt, int screenSizeX, int screenSizeY);
	~Camera();
	virtual const std::shared_ptr<Ray> GetLookingRay(float pixelX, float pixelY) const = 0;
	void GetScreenSize(int & xSize, int & ySize) const;
	virtual void Move(glm::vec3 movement);
	virtual void MoveLookAt(const glm::vec3 & movement);

protected:
	const int screenSizeX;
	const int screenSizeY;
    glm::vec3 direction;
    glm::vec3 lookAt;
	Transform& transform;

};