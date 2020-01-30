#pragma once

#include "SceneObject.h"

#include <glm/glm.hpp>

class Material;
class DiffuseMaterial;
class Shape;
class Sphere;

class PlaneObject : public SceneObject
{
public:
	PlaneObject(Scene & scene, const glm::vec3 & position, const glm::vec3 & size, const glm::vec3 & lookAt, const Material & material);
	virtual ~PlaneObject();

};