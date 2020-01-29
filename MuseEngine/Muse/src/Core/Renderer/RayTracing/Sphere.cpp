#include "MusePCH.h"

#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"

Sphere::Sphere(const vec3 & position, float radius)
	: Shape(position, vec3(radius * 2, radius * 2, radius * 2)), radius(radius) { }

Sphere::~Sphere() {}

bool Sphere::CheckRayHit(vec3 & intersectionPoint, const std::shared_ptr<Ray> ray) const
{
	const vec3 raySphereOffset = transform.GetPosition() - ray->Origin;
	const float offsetLength = ray->Direction.dot(raySphereOffset);

	if (offsetLength < 0) { return false; }

	const vec3 raySphereOffsetOnLine = ray->Origin + ray->Direction * offsetLength;

	const float sphereLocalY = transform.GetPosition().distance(raySphereOffsetOnLine);

	if (sphereLocalY < radius)
	{
		const float sphereLocalX = sqrt(radius * radius - sphereLocalY * sphereLocalY);
		const float lengthToIntersection = offsetLength - sphereLocalX;
		intersectionPoint = ray->Origin + ray->Direction * lengthToIntersection;

		return true;
	}

	return false;
}

vec3 Sphere::GetNormal(const vec3 & point) const
{
	return (point - transform.GetPosition()).normalized();
}

void Sphere::GetMinMaxBounds(vec3 & min, vec3 & max) const
{
	vec3 halfScale = transform.GetScale() / 2;
	min = transform.GetPosition() - halfScale;
	max = transform.GetPosition() + halfScale;
}
