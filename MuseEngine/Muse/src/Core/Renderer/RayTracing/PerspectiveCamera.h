#pragma once

#include "Camera.h"
#include "vec2.h"

struct Ray;

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(const vec3 & position, const vec3 & lookAt, int screenSizeX, int screenSizeY, float vfov, const vec3 & up = vec3(0, 1, 0));
	~PerspectiveCamera();
	virtual const std::shared_ptr<Ray> GetLookingRay(float pixelX, float pixelY) const override;
	virtual void Move(vec3 movement) override;
	virtual void MoveLookAt(const vec3 & movement);

private:
	void UpdateLookingPlane();

	vec3 horizontal;
	vec3 vertical;
	vec3 lowerLeftCorner;
	vec2 size;
	vec3 up;
	vec3 lookAt;

};
	
