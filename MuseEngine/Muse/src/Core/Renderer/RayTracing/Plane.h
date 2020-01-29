#pragma once

#include "Shape.h"
#include "MathLibrary.h"

class Plane: public Shape
{
public:
	Plane(const vec3 & position, const vec3 & normal, const vec3 & size);
	~Plane();
	virtual bool CheckRayHit(vec3 & intersectionPoint, const std::shared_ptr<Ray> ray) const override;
	virtual vec3 GetNormal(const vec3 & point) const override;
	virtual void GetMinMaxBounds(vec3 & min, vec3 & max) const;

private:
	vec3 normal;

};

