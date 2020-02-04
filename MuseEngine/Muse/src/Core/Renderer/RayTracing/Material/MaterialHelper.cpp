#include "MusePCH.h"

#include "MaterialHelper.h"
#include "Core/Renderer/RayTracing/Ray.h"
#include "Core/Renderer/RayTracing/LightSource.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Scene/Scene.h"
#include "Core/Gameplay/Component/RenderComponent.h"

namespace Muse
{
	glm::vec3 operator*(const glm::vec3& a_Lhs, float a_Rhs)
	{
		return glm::vec3(a_Lhs.r * a_Rhs, a_Lhs.g * a_Rhs, a_Lhs.b * a_Rhs);
	}

	void FilterBlockedLights(std::vector<std::shared_ptr<LightSource>>& a_Lights, std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& a_Point)
	{
		const std::vector<std::shared_ptr<RenderComponent>>& renderComponents = RenderComponent::GetAll();

		for (int i = a_Lights.size() - 1; i >= 0; i--)
		{
			const glm::vec3 lightPosition = a_Lights[i]->GetPosition();

			const glm::vec3 directionToLightSource = glm::normalize(lightPosition - a_Point);
			const float maxDistance = (a_Point - lightPosition).length();
			Ray ray;
			ray.Origin = a_Point;
			ray.Direction = directionToLightSource;

			if (ray.Cast())
			{
				a_Lights.erase(a_Lights.begin() + i);
			}
		}
	}
}
