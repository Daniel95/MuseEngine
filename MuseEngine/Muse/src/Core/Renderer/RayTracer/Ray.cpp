#include "MusePCH.h"

#include "Ray.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Renderer/RayTracer/RayHitData.h"

namespace Muse
{
    bool Ray::Cast() const
    {
    #if MUSE_PROFILE
        std::shared_ptr<SceneOld> scene = SceneManagerOld::GetActiveScene();

        scene->IncreaseRaysSend();
    #endif

        const std::vector<std::shared_ptr<RenderComponent>>& renderComponents = RenderComponent::GetAll();

        for (const std::shared_ptr<RenderComponent>& renderComponent : renderComponents)
        {
            const float distance = renderComponent->CheckRayHit(*this);

            if (distance != -1)
            {
            #if MUSE_PROFILE
                scene->IncreaseRaysHit();
            #endif

                return true;
            }
        }

        return false;
    }

    bool Ray::Cast(float a_MaxDistance) const
    {
    #if MUSE_PROFILE
        std::shared_ptr<SceneOld> scene = SceneManagerOld::GetActiveScene();

        scene->IncreaseRaysSend();
    #endif

        const std::vector<std::shared_ptr<RenderComponent>>& renderComponents = RenderComponent::GetAll();

        for (const std::shared_ptr<RenderComponent>& renderComponent : renderComponents)
        {
            const float distance = renderComponent->CheckRayHit(*this);

            if (distance != -1 &&
                distance < a_MaxDistance)
            {
            #if MUSE_PROFILE
                scene->IncreaseRaysHit();
            #endif

                return true;
            }
        }

        return false;
    }

    bool Ray::Cast(const std::shared_ptr<const RenderComponent>& a_Ignore, float a_MaxDistance) const
    {
    #if MUSE_PROFILE
        std::shared_ptr<SceneOld> scene = SceneManagerOld::GetActiveScene();

        scene->IncreaseRaysSend();
    #endif

        const std::vector<std::shared_ptr<RenderComponent>>& renderComponents = RenderComponent::GetAll();

        for (const std::shared_ptr<RenderComponent>& renderComponent : renderComponents)
        {
            if (renderComponent == a_Ignore) { continue; }

            const float distance = renderComponent->CheckRayHit(*this);

            if (distance != -1 &&
                distance < a_MaxDistance)
            {
            #if MUSE_PROFILE
                scene->IncreaseRaysHit();
            #endif

                return true;
            }
        }

        return false;
    }

    bool Ray::Cast(RayHitData& a_RayHitData, const std::shared_ptr<const RenderComponent>& a_Ignore, float a_MaxDistance) const
    {
    #if MUSE_PROFILE
        std::shared_ptr<SceneOld> scene = SceneManagerOld::GetActiveScene();

        scene->IncreaseRaysSend();
    #endif

        const std::vector<std::shared_ptr<RenderComponent>>& renderComponents = RenderComponent::GetAll();

        float closestHitDistance = std::numeric_limits<int>::max();
        std::shared_ptr<RenderComponent> closestHitRenderComponent = nullptr;

        for (const std::shared_ptr<RenderComponent>& renderComponent : renderComponents)
        {
            if (renderComponent == a_Ignore) { continue; }

            const float distance = renderComponent->CheckRayHit(*this);

            if(distance != -1 &&
                distance < closestHitDistance &&
                distance < a_MaxDistance)
            {
                #if MUSE_PROFILE
                    scene->IncreaseRaysHit();
                #endif

                closestHitDistance = distance;
                closestHitRenderComponent = renderComponent;
            }
        }

        a_RayHitData.m_Distance = closestHitDistance;
        a_RayHitData.m_RenderComponent = closestHitRenderComponent;

        return closestHitRenderComponent != nullptr;
    }
}
