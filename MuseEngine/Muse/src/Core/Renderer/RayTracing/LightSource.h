#pragma once

#include <glm/glm.hpp>

namespace Muse
{
	class LightSource
	{
	public:
		LightSource(const glm::vec3& position, const glm::vec3& color, float intensity, float maxRange);
		virtual ~LightSource() = default;
		virtual const glm::vec3& GetLight(const glm::vec3& point) const;
		virtual const glm::vec3& GetDirectionToPoint(const glm::vec3& point) const;
		virtual const glm::vec3& GetPosition() const;

	protected:
		glm::vec3 m_Position;
		float m_Intensity;
		float m_MaxRange;
		glm::vec3 m_Color;
	};
}