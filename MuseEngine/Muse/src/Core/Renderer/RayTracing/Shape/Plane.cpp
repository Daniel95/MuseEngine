#include "MusePCH.h"

#include "Plane.h"
#include "Shape.h"
#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Renderer/RayTracing/Ray.h"


namespace Muse
{
    float Plane::CheckRayHit(const Ray& a_Ray) const
    {
		glm::vec3 position = GetPosition();

		float denom = glm::dot(m_Normal, a_Ray.Direction);
		if (abs(denom) > 0.0001f) // your favorite epsilon
		{
			float t = glm::dot(position - a_Ray.Origin, m_Normal) / denom;
			if (t >= 0) return true; // you might want to allow an epsilon here too
		}
		return false;
    }

	bool Plane::CheckRayHit(glm::vec3& intersectionPoint, const Ray& ray) const
	{
		const float d = glm::dot(ray.Direction, m_Normal);

		if (d < 0)
		{
			const glm::vec3 offset = GetTransform()->GetPosition() - ray.Origin;
			const float yOffset = glm::dot(m_Normal, offset);
			const float t = yOffset / d;

			if (t < 0)
			{
				return false;
			}
			else
			{
				glm::vec3 worldIntersectionPoint = GetTransform()->InverseTransformPoint(ray.Origin + (ray.Direction * t));

				if (abs(worldIntersectionPoint.x) <= 0.5f
					&& abs(worldIntersectionPoint.y) <= 0.5f
					&& abs(worldIntersectionPoint.z) <= 0.5f)
				{
					intersectionPoint = GetTransform()->TransformPoint(worldIntersectionPoint);
					return true;
				}
			}
		}

		return false;
	}

	glm::vec3 Plane::GetNormal(const glm::vec3& a_Point) const
	{
		a_Point;
		return m_Normal;
	}

	void Plane::GetMinMaxBounds(glm::vec3& a_Min, glm::vec3& a_Max) const
	{
		glm::vec3 worldExtends = GetTransform()->TransformVector(glm::vec3(0.5f, 0.5f, 0.5f));
		glm::vec3 shapeMin = GetTransform()->GetPosition() - worldExtends;
		glm::vec3 shapeMax = GetTransform()->GetPosition() + worldExtends;

		a_Min.x = std::min(shapeMin.x, shapeMax.x);
		a_Min.y = std::min(shapeMin.y, shapeMax.y);
		a_Min.z = std::min(shapeMin.z, shapeMax.z);

		a_Max.x = std::max(shapeMin.x, shapeMax.x);
		a_Max.y = std::max(shapeMin.y, shapeMax.y);
		a_Max.z = std::max(shapeMin.z, shapeMax.z);
	}
}
