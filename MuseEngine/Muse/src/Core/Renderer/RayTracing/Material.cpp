#include "MusePCH.h"

#include "Material.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Shape.h"
#include "LightSource.h"

Material::Material(const sf::Color color) : color(color) { }

Material::~Material() { }

const sf::Color & Material::GetColor() const
{
	return color;
}