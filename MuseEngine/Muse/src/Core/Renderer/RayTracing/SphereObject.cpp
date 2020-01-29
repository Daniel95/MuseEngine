#include "MusePCH.h"

#include "SphereObject.h"
#include "Sphere.h"
#include "DiffuseMaterial.h"

SphereObject::SphereObject(Scene& scene, const glm::vec3& position, float radius, const Material& material)
	: SceneObject(scene, material, *new Sphere(position, radius)) { }

SphereObject::~SphereObject() { }