#pragma once

#include "Shape.h"

#include <glm/glm.hpp>

class SceneObject;

class Sphere : public Shape
{
public:
	Sphere(const glm::vec3 & position, float radius);
	~Sphere();
	virtual bool CheckRayHit(glm::vec3 & intersectionPoint, const std::shared_ptr<Ray> ray) const override;
	virtual glm::vec3 GetNormal(const glm::vec3 & point) const override;
	virtual void GetMinMaxBounds(glm::vec3 & min, glm::vec3 & max) const;

private:
	float radius;

};
