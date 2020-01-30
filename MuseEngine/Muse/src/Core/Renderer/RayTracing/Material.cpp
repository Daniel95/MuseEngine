#include "MusePCH.h"

#include "Material.h"
#include "SceneObject.h"
#include "Shape.h"
#include "LightSource.h"

Material::Material(const glm::vec3 color) : color(color) { }

Material::~Material() { }

const glm::vec3 & Material::GetColor() const
{
	return color;
}