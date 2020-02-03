#pragma once

#include <glm/glm.hpp>
#include "Core/Gameplay/Component/Component.h"

namespace Muse
{
	struct Ray;

	class Shape : public Component
	{
	public:
		virtual bool CheckRayHit(glm::vec3& a_IntersectionPoint, const Ray& a_Ray) const = 0;
		virtual glm::vec3 GetNormal(const glm::vec3& a_Point) const = 0;
		virtual void GetMinMaxBounds(glm::vec3& a_Min, glm::vec3& a_Max) const = 0;

	};
}
