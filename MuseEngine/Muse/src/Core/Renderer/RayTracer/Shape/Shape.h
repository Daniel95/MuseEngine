#pragma once

#include <glm/glm.hpp>
#include "Core/Gameplay/Component/Component.h"
#include "Core/Gameplay/Component/TransformComponentOld.h"
#include <memory>

namespace Muse
{
	struct Ray;

	class Shape : public Component
	{
	public:
		virtual float CheckRayHit(const Ray& a_Ray) const = 0;
		virtual glm::vec3 GetNormal(const glm::vec3& a_Point) const = 0;
		virtual glm::vec3 GetRandomPointInShape() const = 0;
		virtual void GetMinMaxBounds(glm::vec3& a_Min, glm::vec3& a_Max) const = 0;
		virtual void OnInit() override;

	protected:
		const glm::vec3& GetPosition() const { return m_TransformComponent->GetLocalPosition(); }

	private:
		std::shared_ptr<TransformComponentOld> m_TransformComponent;

	};
}
