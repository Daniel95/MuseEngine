#pragma once

#include "SceneObject.h"

#include <glm/glm.hpp>

class Material;
class DiffuseMaterial;
class Shape;
class Sphere;

class SphereObject : public SceneObject
{
public:
	SphereObject(Scene & scene, const glm::vec3 & position, float radius, const Material & material);
	virtual ~SphereObject();

};