#include "MusePCH.h"
#include "Render2DJob.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/Render2DComponent.h"
#include "Core/ECS/Component/TransformHelper.h"
#include "Core/Renderer/Renderer2D.h"

#include "entt.hpp"
#include "Core/ECS/Entity/Entity.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/SceneManager.h"

namespace Muse
{
    void Render2DJob::OnUpdate()
    {
        auto func = [](
            int a_Entity,
            TransformComponent& a_TransformComponent,
            Render2DComponent& a_Render2DComponent
            )
        {
            a_Render2DComponent.modelMatrix = TransformHelper::GetWorldModelMatrix(a_TransformComponent);

            if (a_Render2DComponent.texture != nullptr)
            {
                Renderer2D::DrawQuad(a_Render2DComponent.modelMatrix, a_Render2DComponent.texture, a_Render2DComponent.tilingFactor, a_Render2DComponent.color);
            }
            else
            {
                Renderer2D::DrawQuad(a_Render2DComponent.modelMatrix, a_Render2DComponent.color);
            }
        };

        Run<TransformComponent, Render2DComponent>(func);
    }
}
