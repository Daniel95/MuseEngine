#include "MusePCH.h"

#include "LightSource.h"
#include <algorithm>

namespace Muse
{
	LightSource::LightSource(const glm::vec3& a_Position, const glm::vec3& a_Color, float a_Intensity, float a_MaxRange)
		: m_Position(a_Position), m_Intensity(a_Intensity), m_MaxRange(a_MaxRange), m_Color(a_Color)
	{
	}

	glm::vec3 LightSource::GetLight(const glm::vec3& a_Point) const
	{
		const float distanceFromLight = distance(m_Position, a_Point);
		const float falloff = 1 - std::clamp((distanceFromLight / m_MaxRange), 0.0f, 1.0f);
		const glm::vec3 light = (m_Color * m_Intensity) * falloff;
		return light;
	}

	glm::vec3 LightSource::GetDirectionToPoint(const glm::vec3& a_Point) const
	{
		return glm::normalize(a_Point - m_Position);
	}
}