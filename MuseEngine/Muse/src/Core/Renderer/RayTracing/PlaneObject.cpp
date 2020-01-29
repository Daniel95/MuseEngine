#include "MusePCH.h"

#include "PlaneObject.h"
#include "Plane.h"
#include "DiffuseMaterial.h"

PlaneObject::PlaneObject(Scene & scene, const vec3 & position, const vec3 & size, const vec3 & lookAt, const Material & material)
	: SceneObject(scene, material, *new Plane(position, (lookAt - position).normalize(), size)) { }

PlaneObject::~PlaneObject() { }