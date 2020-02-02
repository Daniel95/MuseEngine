﻿#include "GameRT.h"

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

    m_Height = GetViewport()->GetHeight();
    m_Width = GetViewport()->GetHeight();

    //m_Height = 512;
    //m_Width = 512;

    m_ViewportTexture = Muse::ResourceManager::Create<Muse::Texture>("viewPortTexture", m_Width, m_Height);

    m_ScreenData.resize(m_Height * m_Width * 4);

    //scene->ConstructBVH();

    //m_PerspectiveCamera = new Muse::PerspectiveCamera(glm::vec3(0, 0, -3), glm::vec3(0, 0, 0), m_Width, m_Height, 50);
    m_PerspectiveCamera = new Muse::PerspectiveCamera(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), m_Width, m_Height, 50);

    SceneLibraryRT::MakeTestScene(scene);
}

void GameRT::OnUpdate(float a_DeltaTime)
{
    LOG_INFO("DeltaTime: {0}", a_DeltaTime);
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

    //const unsigned int height = 512;
    //const unsigned int width = 512;

    const unsigned int stride = 4;
    const uint32_t size = height * width * stride;

    if(m_Height != height || m_Width != width)
    {
        Resize(width, height);
    }

    std::shared_ptr<Muse::Scene> scene = Muse::SceneManager::GetActiveScene();

    /////////////////

    std::vector<std::shared_ptr<Muse::RayHitData>> rayHitDatas;
    std::shared_ptr<Muse::GetColorParameters> getColorParameters = std::make_shared<Muse::GetColorParameters>();

    glm::mat4 T = glm::mat4(1.0f);

    glm::vec3 p0 = T * glm::vec4(-1, 1, 1, 0); // top-left
    glm::vec3 p1 = T * glm::vec4(1, 1, 1, 0); // top-right
    glm::vec3 p2 = T * glm::vec4(-1, -1, 1, 0); // bottom-left
    glm::vec3 E = T * glm::vec4(0, 0, 0, 1);
    glm::vec3 right = p1 - p0;
    glm::vec3 down = p2 - p0;

    std::shared_ptr<Muse::Ray> ray = std::make_shared<Muse::Ray>();


    int i = 0;
    bool hit = false;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float u = x / static_cast<float>(width);
            float v = y / static_cast<float>(height);
            ray->Origin = p0 + u * right + v * down;
            ray->Direction = glm::normalize(ray->Origin - E);

            scene->RayCast(rayHitDatas, ray);

            if (!rayHitDatas.empty())
            {
                const std::shared_ptr<Muse::RayHitData> closestHit = GetClosestRayHitData(rayHitDatas, ray->Origin);
                getColorParameters->RayDirection = ray->Direction;
                getColorParameters->Bounces = 5;

                const glm::vec3 color = closestHit->m_RenderComponent->GetColor(closestHit->m_IntersectionPoint, getColorParameters);

                m_ScreenData[i] = color.x;
                m_ScreenData[i + 1] = color.y;
                m_ScreenData[i + 2] = color.z;
                m_ScreenData[i + 3] = 1.0f;


                rayHitDatas.clear();
                hit = true;
            }
            i += stride;
        }
    }

    if (!hit)
    {
        LOG_INFO("No hits!");
    }



    /*
    bool hit = false;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            std::shared_ptr<Muse::Ray> ray = m_PerspectiveCamera->GetLookingRay(static_cast<float>(x), static_cast<float>(y));

            if (scene->GetBVH() != nullptr)
            {
                scene->GetBVH()->RayCast(rayHitDatas, ray);
            }
            else
            {
                scene->RayCast(rayHitDatas, ray);
            }

            if (!rayHitDatas.empty())
            {
                const std::shared_ptr<Muse::RayHitData> closestHit = GetClosestRayHitData(rayHitDatas, ray->Origin);
                getColorParameters->RayDirection = ray->Direction;
                getColorParameters->Bounces = 5;

                const glm::vec3 color = closestHit->m_RenderComponent->GetColor(closestHit->m_IntersectionPoint, getColorParameters);

                m_ScreenData[i] = color.x;
                m_ScreenData[i + 1] = color.y;
                m_ScreenData[i + 2] = color.z;
                m_ScreenData[i + 3] = 1.0f;

                i += stride;

                rayHitDatas.clear();

                hit = true;
            }
        }
    }

    if(!hit)
    {
        LOG_INFO("No hits!");
    }
    */

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
