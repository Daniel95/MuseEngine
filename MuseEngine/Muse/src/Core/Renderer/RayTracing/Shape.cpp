#include "MusePCH.h"

#include "Shape.h"
#include "Transform.h"
#include "SceneObject.h"
#include "Ray.h"

Shape::Shape(const glm::vec3 & position, const glm::vec3 & scale)
	: transform(*new Transform(position, scale)) { }

Shape::~Shape() { }

Transform & Shape::GetTransform()
{
	return transform;
}