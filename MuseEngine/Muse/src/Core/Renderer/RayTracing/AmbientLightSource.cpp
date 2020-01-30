#include "MusePCH.h"

#include "AmbientLightSource.h"

namespace Muse
{
	AmbientLightSource::AmbientLightSource(const glm::vec3& color, float intensity)
		: LightSource(glm::vec3(), color, intensity, INFINITY)
	{
	}

	glm::vec3 AmbientLightSource::GetLight(const glm::vec3& point) const
	{
		point;
		return m_Color * m_Intensity;
	}
}