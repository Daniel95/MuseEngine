#include "MusePCH.h"

#include "Sphere.h"
#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Renderer/RayTracing/Ray.h"

namespace Muse
{
	bool Sphere::CheckRayHit(glm::vec3& a_IntersectionPoint, const std::shared_ptr<Ray> a_Ray) const
	{
		const glm::vec3 raySphereOffset = GetTransform()->GetPosition() - a_Ray->Origin;
		const float offsetLength = glm::dot(a_Ray->Direction, raySphereOffset);

		if (offsetLength < 0) { return false; }

		const glm::vec3 raySphereOffsetOnLine = a_Ray->Origin + a_Ray->Direction * offsetLength;

		const float sphereLocalY = glm::distance(GetTransform()->GetPosition(), raySphereOffsetOnLine);

		if (sphereLocalY < m_Radius)
		{
			const float sphereLocalX = sqrt(m_Radius * m_Radius - sphereLocalY * sphereLocalY);
			const float lengthToIntersection = offsetLength - sphereLocalX;
			a_IntersectionPoint = a_Ray->Origin + a_Ray->Direction * lengthToIntersection;

			return true;
		}

		return false;
	}

	glm::vec3 Sphere::GetNormal(const glm::vec3& a_Point) const
	{
		return glm::normalize(a_Point - GetTransform()->GetPosition());
	}

	void Sphere::GetMinMaxBounds(glm::vec3& a_Min, glm::vec3& a_Max) const
	{
		glm::vec3 halfScale = GetTransform()->GetScale() / 2.f;
		a_Min = GetTransform()->GetPosition() - halfScale;
		a_Max = GetTransform()->GetPosition() + halfScale;
	}
}
