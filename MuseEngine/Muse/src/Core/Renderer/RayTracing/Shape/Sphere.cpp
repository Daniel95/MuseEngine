#include "MusePCH.h"

#include "Sphere.h"
#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Renderer/RayTracing/Ray.h"
#include "Core/Utilities/Log.h"

namespace Muse
{
	bool Sphere::CheckRayHit(glm::vec3& a_IntersectionPoint, const Ray& a_Ray) const
	{
		/*
		glm::vec3 C = GetTransform()->GetPosition() - a_Ray->Origin;
		float t = glm::dot(C, a_Ray->Direction);
		glm::vec3 Q = C - t * a_Ray->Direction;
		float p2 = glm::dot(Q, Q);

		float r2 = m_Radius * m_Radius;


		if (p2 > r2) return false; // r2 = r * r
		t -= glm::sqrt(r2 - p2);
		if (t <= 0) { return false; }

		a_IntersectionPoint = a_Ray->Origin + a_Ray->Direction * t;

		return true;
	    */

		//TransformComponent* transformComponent = GetTransformT();
		const glm::vec3 position = GetPosition();

		const glm::vec3 raySphereOffset = position - a_Ray.Origin;
		const float offsetLength = glm::dot(a_Ray.Direction, raySphereOffset);

		if (offsetLength < 0) { return false; }

		const glm::vec3 raySphereOffsetOnLine = a_Ray.Origin + a_Ray.Direction * offsetLength;

		const float sphereLocalY = glm::distance(position, raySphereOffsetOnLine);

		if (sphereLocalY < m_Radius)
		{
			const float sphereLocalX = glm::sqrt(m_Radius * m_Radius - sphereLocalY * sphereLocalY);
			const float lengthToIntersection = offsetLength - sphereLocalX;
			a_IntersectionPoint = a_Ray.Origin + a_Ray.Direction * lengthToIntersection;

			return true;
		}

		return false;
	}

	glm::vec3 Sphere::GetNormal(const glm::vec3& a_Point) const
	{
		glm::vec3 n = glm::normalize(a_Point - GetTransform()->GetPosition());
		return n;
	}

	void Sphere::GetMinMaxBounds(glm::vec3& a_Min, glm::vec3& a_Max) const
	{
        const glm::vec3 halfScale = GetTransform()->GetScale() / 2.f;
		a_Min = GetTransform()->GetPosition() - halfScale;
		a_Max = GetTransform()->GetPosition() + halfScale;
	}
}
