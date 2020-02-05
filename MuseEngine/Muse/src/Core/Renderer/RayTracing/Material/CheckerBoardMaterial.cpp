#include "MusePCH.h"

#include "CheckerBoardMaterial.h"
#include "SpecularMaterial.h"
#include "DiffuseMaterial.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/RayTracing/GetColorParameters.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Renderer/RayTracing/LightSource.h"
#include "Core/Renderer/RayTracing/Ray.h"

namespace Muse
{
	CheckerBoardMaterial::CheckerBoardMaterial(const glm::vec3& a_Color1, const glm::vec3& a_Color2, float a_GridSize, float a_SpeculairStrength)
		: BlinnPhongMaterial(a_Color1, a_SpeculairStrength), m_Color2(a_Color2), m_GridSize(a_GridSize)
	{
	}

	glm::vec3 CheckerBoardMaterial::GetColor(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, const std::shared_ptr<GetColorParameters>& a_GetColorParameters) const
	{
		const glm::vec3 light = GetBlinnPhong(a_RenderComponent, a_Point, a_GetColorParameters);

		int gridPositionX = static_cast<int>(std::round(a_Point.x / m_GridSize));
		int gridPositionY = static_cast<int>(std::round(a_Point.y / m_GridSize));
		int gridPositionZ = static_cast<int>(std::round(a_Point.z / m_GridSize));

		int positionInGrid = gridPositionX + gridPositionY + gridPositionZ;

		glm::vec3 result;

		if (positionInGrid % 2 == 0)
		{
			result = m_Color * light;
		}
		else
		{
			result = m_Color2 * light;
		}

		return result;
	}
}
