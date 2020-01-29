#include "MusePCH.h"

#include "Box.h"
#include "Shape.h"
#include "SceneObject.h"
#include "Transform.h"
#include "Ray.h"
#include "../MathLibrary/vec3.h"

Box::Box(const vec3 & position, const vec3 & scale)
	: Shape(position, scale)
{
	bounds[0] = vec3(-0.5f, -0.5f, -0.5f);
	bounds[1] = vec3(0.5f, 0.5f, 0.5f);
}

Box::~Box() { }

bool Box::CheckRayHit(vec3 & intersectionPoint, const std::shared_ptr<Ray> ray) const
{
	const vec3 localRayDirection = transform.InverseTransformVector(ray->Direction);
	const vec3 localRayOrigin = transform.InverseTransformPoint(ray->Origin);

	float tmin = (bounds[0].m_X - localRayOrigin.m_X) / localRayDirection.m_X;
	float tmax = (bounds[1].m_X - localRayOrigin.m_X) / localRayDirection.m_X;

	if (tmin > tmax)
	{
		std::swap(tmin, tmax);
	}

	float tymin = (bounds[0].m_Y - localRayOrigin.m_Y) / localRayDirection.m_Y;
	float tymax = (bounds[1].m_Y - localRayOrigin.m_Y) / localRayDirection.m_Y;

	if (tymin > tymax)
	{
		std::swap(tymin, tymax);
	}

	if ((tmin > tymax) || (tymin > tmax))
	{
		return false;
	}

	if (tymin > tmin)
	{
		tmin = tymin;
	}

	if (tymax < tmax)
	{
		tmax = tymax;
	}

	float tzmin = (bounds[0].m_Z - localRayOrigin.m_Z) / localRayDirection.m_Z;
	float tzmax = (bounds[1].m_Z - localRayOrigin.m_Z) / localRayDirection.m_Z;

	if (tzmin > tzmax)
	{
		std::swap(tzmin, tzmax);
	}

	if ((tmin > tzmax) || (tzmin > tmax))
	{
		return false;
	}

	if (tzmin > tmin)
	{
		tmin = tzmin;
	}

	if (tzmax < tmax)
	{
		tmax = tzmax;
	}

	if (tmin < 0)
	{
		tmin = tmax;
		if (tmin < 0)
		{
			return false;
		}
	}

	intersectionPoint = ray->Origin + (ray->Direction * tmin);

	return true;
}

vec3 Box::GetNormal(const vec3 & point) const
{
	const vec3 offsetToPoint = transform.InverseTransformPoint(point);
	vec3 normal;

	if (abs(offsetToPoint.m_X) >= abs(offsetToPoint.m_Y) && 
		abs(offsetToPoint.m_X) >= abs(offsetToPoint.m_Z))
	{
		normal = vec3(copysignf(1, offsetToPoint.m_X), 0, 0);
	}
	else if (abs(offsetToPoint.m_Y) >= abs(offsetToPoint.m_X) && 
		abs(offsetToPoint.m_Y) >= abs(offsetToPoint.m_Z))
	{
		normal = vec3(0, copysignf(1, offsetToPoint.m_Y), 0);
	}
	else
	{
		normal = vec3(0, 0, copysignf(1, offsetToPoint.m_Z));
	}

	return transform.TransformVector(normal);
}

void Box::GetMinMaxBounds(vec3 & min, vec3 & max) const
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

