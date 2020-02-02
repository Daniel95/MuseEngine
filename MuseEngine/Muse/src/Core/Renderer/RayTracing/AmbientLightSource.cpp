#include "MusePCH.h"

#include "AmbientLightSource.h"

namespace Muse
{
	AmbientLightSource::AmbientLightSource(const glm::vec3& color, float intensity)
		: LightSource(glm::vec3(), color, intensity, INFINITY)
	{
	}

	const glm::vec3& AmbientLightSource::GetLight(const glm::vec3& point) const
	{
		glm::vec3 light = m_Color * m_Intensity;

		return light;
	}
}