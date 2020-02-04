#include "MusePCH.h"

#include "Plane.h"
#include "Shape.h"
#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Renderer/RayTracing/Ray.h"

namespace Muse
{
    float Plane::CheckRayHit(const Ray& a_Ray) const
    {
		float denom = glm::dot(m_Normal, a_Ray.Direction);
		if (abs(denom) > 0.0001f) // your favorite epsilon
		{
			float t = glm::dot(GetPosition() - a_Ray.Origin, m_Normal) / denom;
			if (t >= 0) return t; // you might want to allow an epsilon here too
		}
		return -1;
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
