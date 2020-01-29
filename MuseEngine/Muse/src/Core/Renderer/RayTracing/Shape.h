#pragma once

#include <SFML/Graphics.hpp>
#include "vec3.h"

class Transform;
struct Ray;

class Shape
{
public:
	Shape(const vec3 & position, const vec3 & scale);
	virtual ~Shape();
	virtual bool CheckRayHit(vec3 & intersectionPoint, const std::shared_ptr<Ray> ray) const = 0;
	virtual vec3 GetNormal(const vec3 & point) const = 0;
	virtual void GetMinMaxBounds(vec3 & min, vec3 & max) const = 0;
	Transform& GetTransform();

protected:
	Transform& transform;

};