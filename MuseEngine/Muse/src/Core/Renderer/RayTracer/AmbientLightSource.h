#pragma once

#include "LightSource.h"

#include <glm/glm.hpp>

namespace Muse
{
	class AmbientLightSource : public LightSource
	{
	public:
		AmbientLightSource(const glm::vec3& a_Color, float a_Intensity);
		virtual ~AmbientLightSource() = default;
		virtual glm::vec3 GetLight(const glm::vec3& a_Point) const override;

	};
}
