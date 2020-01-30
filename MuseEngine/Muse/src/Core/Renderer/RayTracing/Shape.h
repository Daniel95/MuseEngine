#pragma once

#include <glm/glm.hpp>

class Transform;
struct Ray;

class Shape
{
public:
	Shape(const glm::vec3 & position, const glm::vec3 & scale);
	virtual ~Shape();
	virtual bool CheckRayHit(glm::vec3 & intersectionPoint, const std::shared_ptr<Ray> ray) const = 0;
	virtual glm::vec3 GetNormal(const glm::vec3 & point) const = 0;
	virtual void GetMinMaxBounds(glm::vec3 & min, glm::vec3 & max) const = 0;
	Transform& GetTransform();

protected:
	Transform& transform;

};