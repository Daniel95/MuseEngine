#include "MusePCH.h"

#include "LightSource.h"
#include <algorithm>

namespace Muse
{
	LightSource::LightSource(const glm::vec3& position, const glm::vec3& color, float intensity, float maxRange)
		: m_Position(position), m_Intensity(intensity), m_MaxRange(maxRange), m_Color(color)
	{
	}

	const glm::vec3& LightSource::GetLight(const glm::vec3& point) const
	{
		const float distanceFromLight = distance(m_Position, point);
		const float falloff = 1 - std::clamp((distanceFromLight / m_MaxRange), 0.0f, 1.0f);
		const glm::vec3 light = (m_Color * m_Intensity) * falloff;
		return light;
	}

	const glm::vec3& LightSource::GetDirectionToPoint(const glm::vec3& point) const
	{
		return glm::normalize(point - m_Position);
	}

	const glm::vec3& LightSource::GetPosition() const
	{
		return m_Position;
	}
}