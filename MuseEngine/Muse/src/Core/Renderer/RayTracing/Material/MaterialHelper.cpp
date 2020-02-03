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

	void FilterBlockedLights(std::vector<std::shared_ptr<LightSource>>& lights, std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point)
	{
		std::vector<std::shared_ptr<RenderComponent>> renderComponents = RenderComponent::GetAll();
		renderComponents.erase(std::remove(renderComponents.begin(), renderComponents.end(), a_RenderComponent), renderComponents.end());

		for (int i = lights.size() - 1; i >= 0; i--)
		{
			const glm::vec3 lightPosition = lights[i]->GetPosition();

			const glm::vec3 directionToLightSource = glm::normalize(lightPosition - point);
			const float maxDistance = (point - lightPosition).length();
			Ray ray;
			ray.Origin = point;
			ray.Direction = directionToLightSource;

			bool shadow = a_RenderComponent->GetGameObject()->GetScene()->RayCast(renderComponents, ray, maxDistance);

			if (shadow)
			{
				lights.erase(lights.begin() + i);
			}
		}
	}
}
