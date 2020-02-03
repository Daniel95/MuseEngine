#pragma once

#include "Shape.h"

#include <glm/glm.hpp>

namespace Muse
{
	class Plane : public Shape
	{
	public:
		Plane(const glm::vec3& a_Normal);
		virtual ~Plane() = default;
		virtual bool CheckRayHit(glm::vec3& a_IntersectionPoint, const Ray& a_Ray) const override;
		virtual glm::vec3 GetNormal(const glm::vec3& a_Point) const override;
		virtual void GetMinMaxBounds(glm::vec3& a_Min, glm::vec3& a_Max) const override;

	private:
		glm::vec3 normal;

	};

}