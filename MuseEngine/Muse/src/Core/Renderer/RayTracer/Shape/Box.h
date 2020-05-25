#pragma once

#include "Shape.h"
#include "Plane.h"

namespace Muse
{
	class Box : public Shape
	{
	public:
		Box();
		virtual ~Box() = default;
		virtual float CheckRayHit(const Ray& a_Ray) const override;
		virtual glm::vec3 GetNormal(const glm::vec3& a_Point) const override;
		virtual void GetMinMaxBounds(glm::vec3& a_Min, glm::vec3& a_Max) const override;
		virtual glm::vec3 GetRandomPointInShape() const override;

	private:
		glm::vec3 bounds[2];

		//glm::vec3 m_Min = glm::vec3(-0.5f, -0.5f, -0.5f);
		//glm::vec3 m_Max = glm::vec3(0.5f, 0.5f, 0.5f);

	};
}