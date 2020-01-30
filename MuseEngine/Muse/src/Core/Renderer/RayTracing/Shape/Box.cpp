#include "MusePCH.h"

#include "Box.h"
#include "Shape.h"
#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Renderer/RayTracing/Ray.h"

namespace Muse
{
	Box::Box()
	{
		bounds[0] = glm::vec3(-0.5f, -0.5f, -0.5f);
		bounds[1] = glm::vec3(0.5f, 0.5f, 0.5f);
	}

	bool Box::CheckRayHit(glm::vec3& intersectionPoint, const std::shared_ptr<Ray> ray) const
	{
		const glm::vec3 localRayDirection = GetTransform()->InverseTransformVector(ray->Direction);
		const glm::vec3 localRayOrigin = GetTransform()->InverseTransformPoint(ray->Origin);

		float tmin = (bounds[0].x - localRayOrigin.x) / localRayDirection.x;
		float tmax = (bounds[1].x - localRayOrigin.x) / localRayDirection.x;

		if (tmin > tmax)
		{
			std::swap(tmin, tmax);
		}

		float tymin = (bounds[0].y - localRayOrigin.y) / localRayDirection.y;
		float tymax = (bounds[1].y - localRayOrigin.y) / localRayDirection.y;

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

		float tzmin = (bounds[0].z - localRayOrigin.z) / localRayDirection.z;
		float tzmax = (bounds[1].z - localRayOrigin.z) / localRayDirection.z;

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

	glm::vec3 Box::GetNormal(const glm::vec3& point) const
	{
		const glm::vec3 offsetToPoint = GetTransform()->InverseTransformPoint(point);
		glm::vec3 normal;

		if (abs(offsetToPoint.x) >= abs(offsetToPoint.y) &&
			abs(offsetToPoint.x) >= abs(offsetToPoint.z))
		{
			normal = glm::vec3(copysignf(1, offsetToPoint.x), 0, 0);
		}
		else if (abs(offsetToPoint.y) >= abs(offsetToPoint.x) &&
			abs(offsetToPoint.y) >= abs(offsetToPoint.z))
		{
			normal = glm::vec3(0, copysignf(1, offsetToPoint.y), 0);
		}
		else
		{
			normal = glm::vec3(0, 0, copysignf(1, offsetToPoint.z));
		}

		return GetTransform()->TransformVector(normal);
	}

	void Box::GetMinMaxBounds(glm::vec3& min, glm::vec3& max) const
	{
		glm::vec3 worldExtends = GetTransform()->TransformVector(glm::vec3(0.5f, 0.5f, 0.5f));
		glm::vec3 shapeMin = GetTransform()->GetPosition() - worldExtends;
		glm::vec3 shapeMax = GetTransform()->GetPosition() + worldExtends;

		min.x = std::min(shapeMin.x, shapeMax.x);
		min.y = std::min(shapeMin.y, shapeMax.y);
		min.z = std::min(shapeMin.z, shapeMax.z);

		max.x = std::max(shapeMin.x, shapeMax.x);
		max.y = std::max(shapeMin.y, shapeMax.y);
		max.z = std::max(shapeMin.z, shapeMax.z);
	}

}
