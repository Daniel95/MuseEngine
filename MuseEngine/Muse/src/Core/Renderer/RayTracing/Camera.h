#pragma once

#include "MathLibrary.h"
#include "Transform.h"
#include "Ray.h"

class Camera
{
public:
	Camera(const vec3 & position, const vec3 & lookAt, int screenSizeX, int screenSizeY);
	~Camera();
	virtual const std::shared_ptr<Ray> GetLookingRay(float pixelX, float pixelY) const = 0;
	void GetScreenSize(int & xSize, int & ySize) const;
	virtual void Move(vec3 movement);
	virtual void MoveLookAt(const vec3 & movement);

protected:
	const int screenSizeX;
	const int screenSizeY;
	vec3 direction;
	vec3 lookAt;
	Transform& transform;

};