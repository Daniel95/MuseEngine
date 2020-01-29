#pragma once

#include <SFML/Graphics.hpp>

#include "SceneObject.h"

class Material;
class DiffuseMaterial;
class Shape;
class Sphere;

class SphereObject : public SceneObject
{
public:
	SphereObject(Scene & scene, const vec3 & position, float radius, const Material & material);
	virtual ~SphereObject();

};