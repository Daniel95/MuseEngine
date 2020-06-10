﻿#include "GameRT.h"

#include "Muse.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "Core/Resource/ResourceTest.h"
#include "Core/Window.h"

#include "Mode.h"
#include "Editor/ViewPort.h"
#include "Core/Renderer/Buffer/FrameBuffer.h"
#include "Core/Renderer/RayTracer/RayHitData.h"
#include "Core/Renderer/RayTracer/GetColorParameters.h"
#include "Core/Renderer/RayTracer/BVH/BVH.h"
#include "Core/Renderer/RayTracer/Ray.h"
#include "RayTracer/SceneLibraryRT.h"
#include "Core/Gameplay/Component/CameraComponent.h"
#include "Core/Renderer/RayTracer/RendererRT.h"

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

    std::shared_ptr <Muse::Scene> scene = Muse::Scene::Create();
    Muse::ResourceManager::Add("GameRTScene", scene);
    Muse::SceneManager::SwitchScene(scene);

    m_Height = GetViewport()->GetHeight();
    m_Width = GetViewport()->GetHeight();

    m_ScreenData.resize(m_Height * m_Width * 4);

    //scene->ConstructBVH();

    SceneLibraryRT::MakeRTBenchmarkScene(scene);
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

    Muse::CameraComponent* camera = Muse::CameraComponent::GetMain();

    Muse::Renderer2D::BeginScene(*camera);

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
    Muse::GetColorParameters getColorParameters;

    glm::mat4 T = camera->GetTransform()->GetWorldModelMatrix();

    glm::vec3 p0 = T * glm::vec4(-1, -1, 1, 0); // top-left
    glm::vec3 p1 = T * glm::vec4(1, -1, 1, 0); // top-right
    glm::vec3 p2 = T * glm::vec4(-1, 1, 1, 0); // bottom-left

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

            ray.Origin = p0 + u * right + v * down + E;
            ray.Direction = glm::normalize(ray.Origin - E);

            if(ray.Cast(rayHitData))
            {
                getColorParameters.Bounces = 5;
                getColorParameters.Ray = &ray;
                getColorParameters.IntersectionPoint = rayHitData.UpdateIntersectionPoint(ray);
                getColorParameters.RenderComponent = rayHitData.m_RenderComponent;

                const glm::vec3 color = Muse::RendererRT::CalculateColor(getColorParameters);

                m_ScreenData[colorIndex] = color.x;
                m_ScreenData[colorIndex + 1] = color.y;
                m_ScreenData[colorIndex + 2] = color.z;
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
