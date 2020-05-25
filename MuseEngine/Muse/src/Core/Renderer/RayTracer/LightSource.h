#pragma once

#include <glm/glm.hpp>

namespace Muse
{
	class LightSource
	{
	public:
		LightSource(const glm::vec3& a_Position, const glm::vec3& a_Color, float a_Intensity, float a_MaxRange);
		virtual ~LightSource() = default;
		virtual glm::vec3 GetLight(const glm::vec3& a_Point) const;
		virtual glm::vec3 GetDirectionToPoint(const glm::vec3& a_Point) const;
		virtual const glm::vec3& GetPosition() const { return m_Position; };

	protected:
		glm::vec3 m_Position;
		float m_Intensity;
		float m_MaxRange;
		glm::vec3 m_Color;
	};
}