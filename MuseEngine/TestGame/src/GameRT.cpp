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
#include "Core/Renderer/RayTracing/RayHitData.h"
#include "Core/Renderer/RayTracing/GetColorParameters.h"
#include "Core/Renderer/RayTracing/BVH/BVH.h"
#include "Core/Renderer/RayTracing/Ray.h"
#include "Core/Renderer/RayTracing/PerspectiveCamera.h"
#include "RayTracer/SceneLibraryRT.h"
#include "Core/Gameplay/Component/CameraComponent.h"

#if GAME_RT
#include "EntryPoint.h"
Muse::Application* Muse::CreateApplication()
{
    return new GameRT();
}
#endif

void GameRT::OnStart()
{
    MUSE_PROFILE_FUNCTION();

    Muse::RenderCommand::Init();
    Muse::Renderer2D::Init();

    std::shared_ptr<Muse::Scene> scene = Muse::ResourceManager::Create<Muse::Scene>("New Scene");
    Muse::SceneManager::SwitchScene(scene);

    m_Height = GetViewport()->GetHeight();
    m_Width = GetViewport()->GetHeight();

    m_ViewportTexture = Muse::ResourceManager::Create<Muse::Texture>("viewPortTexture", m_Width, m_Height);

    m_ScreenData.resize(m_Height * m_Width * 4);

    //scene->ConstructBVH();

    SceneLibraryRT::MakeTestScene(scene);
}

void GameRT::OnUpdate(float a_DeltaTime)
{
}

void GameRT::OnFixedUpdate()
{
}

void GameRT::OnRender()
{
    MUSE_PROFILE_FUNCTION();

    Muse::RenderCommand::SetClearColor({ 1.f, 1.0f, 1.0f, 1 });
    Muse::RenderCommand::Clear();

    std::shared_ptr<Muse::CameraComponent> camera = Muse::CameraComponent::GetMain();

    Muse::Renderer2D::BeginScene(camera);

    const unsigned int height = GetViewport()->GetHeight();
    const unsigned int width = GetViewport()->GetWidth();

    const unsigned int stride = 4;
    const uint32_t size = height * width * stride;

    if(m_Height != height || m_Width != width)
    {
        Resize(width, height);
    }

    int setAlphaColorIndex = 0;

    for (uint32_t i = 0; i < height * width; i++)
    {
        m_ScreenData[setAlphaColorIndex + 3] = 1.0f;
        setAlphaColorIndex += stride;
    }

    std::shared_ptr<Muse::Scene> scene = Muse::SceneManager::GetActiveScene();

    /////////////////

    const glm::vec3 backgroundColor = scene->GetBackgroundColor();

    Muse::RayHitData rayHitData;
    std::shared_ptr<Muse::GetColorParameters> getColorParameters = std::make_shared<Muse::GetColorParameters>();

    glm::mat4 T = camera->GetTransform()->GetModelMatrix();

    glm::vec3 p0 = T * glm::vec4(-1, -1, 1, 0); // top-left
    glm::vec3 p1 = T * glm::vec4(1, -1, 1, 0); // top-right
    glm::vec3 p2 = T * glm::vec4(-1, 1, 1, 0); // bottom-left

    /*
    glm::vec3 p0 = T * glm::vec4(-1, 1, 1, 0); // top-left
    glm::vec3 p1 = T * glm::vec4(1, 1, 1, 0); // top-right
    glm::vec3 p2 = T * glm::vec4(-1, -1, 1, 0); // bottom-left
    */

    glm::vec3 E = T * glm::vec4(0, 0, 0, 1);
    glm::vec3 right = p1 - p0;
    glm::vec3 down = p2 - p0;

    Muse::Ray ray;

    int colorIndex = 0;
    bool hit = false;
    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            float u = x / static_cast<float>(width);
            float v = y / static_cast<float>(height);
            ray.Origin = p0 + u * right + v * down;
            ray.Direction = glm::normalize(ray.Origin - E);

            if(ray.Cast(rayHitData))
            {
                rayHitData.UpdateIntersectionPoint(ray);

                getColorParameters->RayDirection = ray.Direction;
                getColorParameters->Bounces = 5;

                const glm::vec3 color = rayHitData.m_RenderComponent->GetColor(rayHitData.GetIntersectionPoint(), getColorParameters);

                /*
                m_ScreenData[i] = std::sqrt(color.x);
                m_ScreenData[i + 1] = std::sqrt(color.y);
                m_ScreenData[i + 2] = std::sqrt(color.z);
                m_ScreenData[i + 3] = 1.0f;
                */

                m_ScreenData[colorIndex] = color.x;
                m_ScreenData[colorIndex + 1] = color.y;
                m_ScreenData[colorIndex + 2] = color.z;

                hit = true;
            }
            else
            {
                m_ScreenData[colorIndex] = backgroundColor.x;
                m_ScreenData[colorIndex + 1] = backgroundColor.y;
                m_ScreenData[colorIndex + 2] = backgroundColor.z;
            }

            colorIndex += stride;
        }
    }

    if (!hit)
    {
        LOG_INFO("No hits!");
    }

    GetViewport()->BindTexture();
    GetViewport()->SetDataF(&m_ScreenData[0], size);

    Muse::Renderer2D::EndScene();
}

void GameRT::OnImGuiRender()
{
    ImGui::Begin("Info");

    ImGui::Text("FPS: %f", 1 / GetDeltaTime());

    std::shared_ptr<Muse::Scene> scene = Muse::SceneManager::GetActiveScene();

    const int raysSend = scene->GetRaysSend();
    const int raysHit = scene->GetRaysHit();

    float raysHitRate = 0;
    if(raysSend != 0 && raysHit != 0)
    {
        raysHitRate = static_cast<float>(raysHit) / static_cast<float>(raysSend);
    }

    ImGui::Text("Rays send: %i", raysSend);
    ImGui::Text("Rays hit: %i", raysHit);
    ImGui::Text("Rays hit rate: %f", raysHitRate);

    scene->ResetRaysHit();
    scene->ResetRaysSend();

    ImGui::End();
}

void GameRT::Resize(unsigned a_Width, unsigned a_Height)
{
    m_ScreenData.clear();
    m_ScreenData.resize(a_Height * a_Width * 4);

    m_Height = a_Height;
    m_Width = a_Width;
}
