#include "MusePCH.h"
#include "Render2DJob.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/Render2DComponent.h"
#include "Core/ECS/Component/TransformHelper.h"
#include "Core/Renderer/Renderer2D.h"

namespace Muse
{
    void Render2DJob::OnUpdate()
    {
        /*
        auto func = [](
            const std::vector<int>& a_Entities,
            std::unordered_map<int, TransformComponent>& a_TransformComponents,
            std::unordered_map<int, Render2DComponent>& a_Render2DComponents
            )
        {
            for (int entity : a_Entities)
            {
                auto transformIt = a_TransformComponents.find(entity);
                auto renderIt = a_Render2DComponents.find(entity);

                renderIt->second.modelMatrix = TransformHelper::GetWorldModelMatrix(transformIt->second);

                if (renderIt->second.texture != nullptr)
                {
                    Renderer2D::DrawQuad(renderIt->second.modelMatrix, renderIt->second.texture, renderIt->second.tilingFactor, renderIt->second.color);
                }
                else
                {
                    Renderer2D::DrawQuad(renderIt->second.modelMatrix, renderIt->second.color);
                }
            }
        };

        auto transforms = Muse::ComponentManager<Muse::TransformComponent>::GetComponents();
        auto render2DComponents = Muse::ComponentManager<Muse::Render2DComponent>::GetComponents();
        */

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
