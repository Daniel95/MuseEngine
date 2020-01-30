#include "MusePCH.h"

#include "Plane.h"
#include "Shape.h"
#include "SceneObject.h"
#include "Transform.h"
#include "Ray.h"


namespace Muse
{

	Plane::Plane(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& size)
		: Shape(position, size), normal(normal)
	{
	}

	Plane::~Plane() {}

	bool Plane::CheckRayHit(glm::vec3& intersectionPoint, std::shared_ptr<Ray> ray) const
	{
		const float d = ray->Direction.dot(normal);

		if (d < 0)
		{
			const glm::vec3 offset = m_Transform.GetPosition() - ray->Origin;
			const float yOffset = normal.dot(offset);
			const float t = yOffset / d;

			if (t < 0)
			{
				return false;
			}
			else
			{
				glm::vec3 worldIntersectionPoint = m_Transform.InverseTransformPoint(ray->Origin + (ray->Direction * t));

				if (abs(worldIntersectionPoint.x) <= 0.5f
					&& abs(worldIntersectionPoint.y) <= 0.5f
					&& abs(worldIntersectionPoint.z) <= 0.5f)
				{
					intersectionPoint = m_Transform.TransformPoint(worldIntersectionPoint);
					return true;
				}
			}
		}

		return false;
	}

	glm::vec3 Plane::GetNormal(const glm::vec3& point) const
	{
		point;
		return normal;
	}

	void Plane::GetMinMaxBounds(glm::vec3& min, glm::vec3& max) const
	{
		glm::vec3 worldExtends = m_Transform.TransformVector(glm::vec3(0.5f, 0.5f, 0.5f));
		glm::vec3 shapeMin = m_Transform.GetPosition() - worldExtends;
		glm::vec3 shapeMax = m_Transform.GetPosition() + worldExtends;

		min.x = std::min(shapeMin.x, shapeMax.x);
		min.y = std::min(shapeMin.y, shapeMax.y);
		min.z = std::min(shapeMin.z, shapeMax.z);

		max.x = std::max(shapeMin.x, shapeMax.x);
		max.y = std::max(shapeMin.y, shapeMax.y);
		max.z = std::max(shapeMin.z, shapeMax.z);
	}
}