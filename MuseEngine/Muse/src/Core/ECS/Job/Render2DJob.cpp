#include "MusePCH.h"
#include "RenderJob.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/Render2DComponent.h"
#include "Core/ECS/Component/TransformHelper.h"
#include "Core/Renderer/Renderer2D.h"

namespace Muse
{
    void Render2DJob::OnUpdate()
    {
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
            }
        };

        Run<TransformComponent, Render2DComponent>(func);
    }
}
