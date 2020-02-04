#include "MusePCH.h"

#include "CheckerBoardMaterial.h"
#include "SpecularMaterial.h"
#include "DiffuseMaterial.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/RayTracing/GetColorParameters.h"
#include "Core/Gameplay/Component/RenderComponent.h"

namespace Muse
{
	CheckerBoardMaterial::CheckerBoardMaterial(const glm::vec3& a_Color1, const glm::vec3& a_Color2, float a_GridSize, float a_SpeculairStrength)
		: BlinnPhongMaterial(a_Color1, a_SpeculairStrength), m_Color2(a_Color2), m_GridSize(a_GridSize)
	{
	}

	glm::vec3 CheckerBoardMaterial::GetColor(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const
	{
		const glm::vec3 speculair = m_SpeculairMaterial.GetSpecular(a_RenderComponent, point, getColorParameters->RayDirection);
		const glm::vec3 diffuse = m_DiffuseMaterial.GetDiffuse(a_RenderComponent, point);
		const glm::vec3 combinedLights = speculair + diffuse + a_RenderComponent->GetGameObject()->GetScene()->GetAmbientLight();

		int gridPositionX = static_cast<int>(std::round(point.x / m_GridSize));
		int gridPositionY = static_cast<int>(std::round(point.y / m_GridSize));
		int gridPositionZ = static_cast<int>(std::round(point.z / m_GridSize));

		int positionInGrid = gridPositionX + gridPositionY + gridPositionZ;

		glm::vec3 result;

		if (positionInGrid % 2 == 0)
		{
			result = m_Color * combinedLights;
		}
		else
		{
			result = m_Color2 * combinedLights;
		}

		return result;
	}
}
