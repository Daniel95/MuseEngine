#include "MusePCH.h"

#include "NormalDebugMaterial.h"
#include "MaterialHelper.h"
#include "Core/Gameplay/Component/RenderComponent.h"

namespace Muse
{
	NormalDebugMaterial::NormalDebugMaterial(const glm::vec3& a_Color)
		: m_Color(a_Color)
	{
	}

	glm::vec3 NormalDebugMaterial::GetColor(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, const std::shared_ptr<GetColorParameters>& a_GetColorParameters) const
	{
		a_GetColorParameters;

		const glm::vec3 normal = (a_RenderComponent->GetNormal(a_Point) + glm::vec3(1, 1, 1)) / 2.f;
		const glm::vec3 normalDebugColor = glm::vec3(normal.x * m_Color.r, normal.y * m_Color.g, normal.z * m_Color.b);
		return normalDebugColor;
	}
}
