#pragma once

#include "LightSource.h"

#include <glm/glm.hpp>

namespace Muse
{
	class AmbientLightSource : public LightSource
	{
	public:
		AmbientLightSource(const glm::vec3& color, float intensity);
		virtual ~AmbientLightSource() = default;
		virtual const glm::vec3& GetLight(const glm::vec3& point) const override;

	};
}
