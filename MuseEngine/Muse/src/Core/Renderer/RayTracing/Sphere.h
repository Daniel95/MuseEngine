#pragma once

#include "MathLibrary.h"
#include "Shape.h"

class SceneObject;

class Sphere : public Shape
{
public:
	Sphere(const vec3 & position, float radius);
	~Sphere();
	virtual bool CheckRayHit(vec3 & intersectionPoint, const std::shared_ptr<Ray> ray) const override;
	virtual vec3 GetNormal(const vec3 & point) const override;
	virtual void GetMinMaxBounds(vec3 & min, vec3 & max) const;

private:
	float radius;

};
