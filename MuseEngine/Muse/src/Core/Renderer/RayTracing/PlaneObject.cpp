#include "MusePCH.h"

#include "PlaneObject.h"
#include "Plane.h"
#include "DiffuseMaterial.h"

PlaneObject::PlaneObject(Scene & scene, const glm::vec3 & position, const glm::vec3 & size, const glm::vec3 & lookAt, const Material & material)
	: SceneObject(scene, material, *new Plane(position, (lookAt - position).normalize(), size)) { }

PlaneObject::~PlaneObject() { }