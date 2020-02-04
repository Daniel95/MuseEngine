#pragma once

#include "Shape.h"

#include <glm/glm.hpp>

namespace Muse
{
	class Plane : public Shape
	{
	public:
		Plane() = default;
		virtual ~Plane() = default;
		virtual float CheckRayHit(const Ray& a_Ray) const override;
		virtual glm::vec3 GetNormal(const glm::vec3& a_Point) const override;
		virtual void SetNormal(const glm::vec3& a_Normal) { m_Normal = a_Normal; }
		virtual void GetMinMaxBounds(glm::vec3& a_Min, glm::vec3& a_Max) const override;

	private:
		glm::vec3 m_Normal;

	};

}