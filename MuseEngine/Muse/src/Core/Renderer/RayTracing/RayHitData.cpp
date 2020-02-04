#include "MusePCH.h"

#include "RayHitData.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Utilities/Defines.h"
#include "Ray.h"

namespace Muse
{
	const glm::vec3& RayHitData::GetIntersectionPoint() const
	{
		ASSERT_ENGINE(m_IntersectionPoint != glm::vec3(std::numeric_limits<float>::max()), "IntersectionPoint has not been updated!");

		return m_IntersectionPoint;
	}

	const glm::vec3& RayHitData::UpdateIntersectionPoint(const Ray& a_Ray)
	{
		m_IntersectionPoint = a_Ray.Origin + a_Ray.Direction * m_Distance;
		return m_IntersectionPoint;
	}
}
