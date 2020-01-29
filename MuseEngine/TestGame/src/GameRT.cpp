#include "GameRT.h"

#include "Muse.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "PlayerComponent.h"
#include "Core/Resource/ResourceTest.h"
#include "Core/Window.h"

#include "Mode.h"
#include "Editor/ViewPort.h"
#include "Core/Renderer/Buffer/FrameBuffer.h"

#if GAME_RT
#include "EntryPoint.h"
Muse::Application* Muse::CreateApplication()
{
    return new GameRT();
}
#endif

void GameRT::OnStart()
{
    Muse::RenderCommand::Init();
    Muse::Renderer2D::Init();

    std::shared_ptr<Muse::Scene> scene = Muse::ResourceManager::Create<Muse::Scene>("New Scene");
    Muse::SceneManager::SwitchScene(scene);
    m_Test = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/rayman.png");

    m_Height = GetViewport()->GetHeight();
    m_Width = GetViewport()->GetWidth();

    m_ViewportTexture = Muse::ResourceManager::Create<Muse::Texture>("viewPortTexture", m_Width, m_Height);

    m_ScreenData.resize(m_Height * m_Width * 4);
}

void GameRT::OnUpdate(float a_DeltaTime)
{
}

void GameRT::OnFixedUpdate()
{
}

void GameRT::OnRender()
{
    Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::Clear();

    Muse::Renderer2D::BeginScene(*Muse::CameraComponent::GetMain());

    const unsigned int height = GetViewport()->GetHeight();
    const unsigned int width = GetViewport()->GetWidth();

    const unsigned int stride = 4;
    const uint32_t size = height * width * stride;

    if(m_Height != height || m_Width != width)
    {
        Resize(width, height);
    }

    /////////////////

    std::vector<std::shared_ptr<RayHitData>> rayHitDatas;
    std::shared_ptr<GetColorParameters> getColorParameters = std::make_shared<GetColorParameters>();

    for (int y = 0; y < screenSizeY; y++)
    {
        for (int x = 0; x < screenSizeX; x++)
        {
            std::shared_ptr<Ray> ray = camera.GetLookingRay(static_cast<float>(x), static_cast<float>(y));

            if (bvh != NULL)
            {
                bvh->RayCast(rayHitDatas, ray);
            }
            else
            {
                RayCast(rayHitDatas, ray);
            }

            if (rayHitDatas.size() > 0)
            {
                const std::shared_ptr<RayHitData> closestHit = GetClosestRayHitData(rayHitDatas, ray->Origin);
                getColorParameters->RayDirection = ray->Direction;
                getColorParameters->Bounces = 5;

                const sf::Color color = closestHit->HitSceneObject->GetColor(closestHit->IntersectionPoint, getColorParameters);
                image.setPixel(x, y, color);

                rayHitDatas.clear();
            }
        }
    }




    ////////////




    /*
    glm::mat4 t = glm::mat4(1);

    glm::vec3 p0 = t * glm::vec4(-1, 1, 1, 0);
    glm::vec3 p0 = t * glm::vec4(-1, 1, 1, 0);
    glm::vec3 p0 = t * glm::vec4(-1, 1, 1, 0);
    */

    glm::vec3 color = glm::vec3(1, 0.2f, 0.2f);

    int i = 0;
    for (int y = 0; y < m_Height; ++y)
    {
        for (int x = 0; x < m_Width; ++x)
        {
            m_ScreenData[i] = color.x;
            m_ScreenData[i + 1] = color.y;
            m_ScreenData[i + 2] = color.z;
            m_ScreenData[i + 3] = 1.0f;

            i += stride;
        }
    }
    GetViewport()->BindTexture();
    GetViewport()->SetDataF(&m_ScreenData[0], size);

    Muse::Renderer2D::EndScene();
}

void GameRT::OnImGuiRender()
{
}

void GameRT::Resize(unsigned a_Width, unsigned a_Height)
{
    m_ScreenData.clear();
    m_ScreenData.resize(a_Height * a_Width * 4);

    m_Height = a_Height;
    m_Width = a_Width;
}
