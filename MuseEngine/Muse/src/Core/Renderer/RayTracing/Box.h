#pragma once

#include "Shape.h"
#include "Plane.h"
#include "MathLibrary.h"

class Box : public Shape
{
public:
	Box(const vec3 & position = vec3(0, 0, 0), const vec3 & scale = vec3(1, 1, 1));
	~Box();
	virtual bool CheckRayHit(vec3& intersectionPoint, const std::shared_ptr<Ray> ray) const override;
	virtual vec3 GetNormal(const vec3 & point) const override;
	virtual void GetMinMaxBounds(vec3 & min, vec3 & max) const;

private:
	vec3 bounds[2];
};

