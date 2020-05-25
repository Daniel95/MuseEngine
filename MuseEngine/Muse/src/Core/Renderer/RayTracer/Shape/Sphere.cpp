#include "MusePCH.h"

#include "Sphere.h"
#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Renderer/RayTracer/Ray.h"
#include "Core/Utilities/Log.h"

namespace Muse
{
    float Sphere::CheckRayHit(const Ray& a_Ray) const
    {
		glm::vec3 C = GetPosition() - a_Ray.Origin;
		float t = dot(C, a_Ray.Direction);
		glm::vec3 Q = C - t * a_Ray.Direction;
		float p2 = dot(Q, Q);
		if (p2 > m_Radius2) return - 1; // r2 = r * r
		t -= sqrt(m_Radius2 - p2);
		//if ((t < ray.t) && (t > 0)) ray.t = t;
		if (t > 0) return t;
		return -1;
    }

	glm::vec3 Sphere::GetNormal(const glm::vec3& a_Point) const
	{
		glm::vec3 n = glm::normalize(a_Point - GetTransform()->GetLocalPosition());
		return n;
	}

	void Sphere::GetMinMaxBounds(glm::vec3& a_Min, glm::vec3& a_Max) const
	{
        const glm::vec3 halfScale = GetTransform()->GetLocalScale() / 2.f;
		a_Min = GetTransform()->GetLocalPosition() - halfScale;
		a_Max = GetTransform()->GetLocalPosition() + halfScale;
	}

    void Sphere::SetRadius(float a_Radius)
    {
		m_Radius = a_Radius;
		m_Radius2 = a_Radius * a_Radius;
    }

    glm::vec3 Sphere::GetRandomPointInShape() const
    {
		return glm::vec3(0);
    }
}
