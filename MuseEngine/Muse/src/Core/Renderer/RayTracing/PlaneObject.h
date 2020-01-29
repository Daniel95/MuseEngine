#pragma once

#include <SFML/Graphics.hpp>

#include "SceneObject.h"

class Material;
class DiffuseMaterial;
class Shape;
class Sphere;

class PlaneObject : public SceneObject
{
public:
	PlaneObject(Scene & scene, const vec3 & position, const vec3 & size, const vec3 & lookAt, const Material & material);
	virtual ~PlaneObject();

};