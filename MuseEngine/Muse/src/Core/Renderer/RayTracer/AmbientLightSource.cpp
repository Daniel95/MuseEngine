#include "MusePCH.h"

#include "AmbientLightSource.h"

namespace Muse
{
	AmbientLightSource::AmbientLightSource(const glm::vec3& a_Color, float a_Intensity)
		: LightSource(glm::vec3(), a_Color, a_Intensity, INFINITY)
	{
	}

	glm::vec3 AmbientLightSource::GetLight(const glm::vec3& point) const
	{
		glm::vec3 light = m_Color * m_Intensity;

		return light;
	}
}