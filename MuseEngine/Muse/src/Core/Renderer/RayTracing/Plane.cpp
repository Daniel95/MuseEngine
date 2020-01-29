#include "MusePCH.h"

#include "Plane.h"
#include "Shape.h"
#include "SceneObject.h"
#include "Transform.h"
#include "Ray.h"

Plane::Plane(const vec3 & position, const vec3 & normal, const vec3 & size)
	: Shape(position, size), normal(normal) { }

Plane::~Plane() { }

bool Plane::CheckRayHit(vec3 & intersectionPoint, std::shared_ptr<Ray> ray) const
{
	const float d = ray->Direction.dot(normal);

	if (d < 0)
	{
		const vec3 offset = transform.GetPosition() - ray->Origin;
		const float yOffset = normal.dot(offset);
		const float t = yOffset / d;

		if (t < 0)
		{
			return false;
		}
		else
		{
			vec3 worldIntersectionPoint = transform.InverseTransformPoint(ray->Origin + (ray->Direction * t));

			if (abs(worldIntersectionPoint.m_X) <= 0.5f
				&& abs(worldIntersectionPoint.m_Y) <= 0.5f
				&& abs(worldIntersectionPoint.m_Z) <= 0.5f)
			{
				intersectionPoint = transform.TransformPoint(worldIntersectionPoint);
				return true;
			}
		}
	}

	return false;
}

vec3 Plane::GetNormal(const vec3 & point) const
{
	point;
	return normal;
}

void Plane::GetMinMaxBounds(vec3 & min, vec3 & max) const
{
	vec3 worldExtends = transform.TransformVector(vec3(0.5f, 0.5f, 0.5f));
	vec3 shapeMin = transform.GetPosition() - worldExtends;
	vec3 shapeMax = transform.GetPosition() + worldExtends;

	min.m_X = std::min(shapeMin.m_X, shapeMax.m_X);
	min.m_Y = std::min(shapeMin.m_Y, shapeMax.m_Y);
	min.m_Z = std::min(shapeMin.m_Z, shapeMax.m_Z);

	max.m_X = std::max(shapeMin.m_X, shapeMax.m_X);
	max.m_Y = std::max(shapeMin.m_Y, shapeMax.m_Y);
	max.m_Z = std::max(shapeMin.m_Z, shapeMax.m_Z);
}