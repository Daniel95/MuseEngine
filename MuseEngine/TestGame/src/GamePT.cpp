#include "GamePT.h"

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
#include "RayTracer/SceneLibraryRT.h"
#include "Core/Gameplay/Component/CameraComponent.h"

#if GAME_PT
#include "EntryPoint.h"
Muse::Application* Muse::CreateApplication()
{
    return new GamePT();
}
#endif

void GamePT::OnStart()
{
    MUSE_PROFILE_FUNCTION();

    Muse::RenderCommand::Init();
    Muse::Renderer2D::Init();

    std::shared_ptr<Muse::Scene> scene = Muse::ResourceManager::Create<Muse::Scene>("New Scene");
    Muse::SceneManager::SwitchScene(scene);

    m_Height = GetViewport()->GetHeight();
    m_Width = GetViewport()->GetHeight();

    m_ScreenData.resize(m_Height * m_Width * 4);

    //scene->ConstructBVH();

    SceneLibraryRT::MakePTBenchmarkScene(scene);
}

void GamePT::OnUpdate(float a_DeltaTime)
{
}

void GamePT::OnFixedUpdate()
{
}

void GamePT::OnRender()
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

    if (m_Height != height || m_Width != width)
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

    glm::mat4 T = camera->GetTransform()->GetModelMatrix();

    glm::vec3 p0 = T * glm::vec4(-1, -1, 1, 0); // top-left
    glm::vec3 p1 = T * glm::vec4(1, -1, 1, 0); // top-right
    glm::vec3 p2 = T * glm::vec4(-1, 1, 1, 0); // bottom-left

    glm::vec3 E = T * glm::vec4(0, 0, 0, 1);
    glm::vec3 right = p1 - p0;
    glm::vec3 down = p2 - p0;

    Muse::Ray ray;
    m_Hit = false;

    int colorIndex = 0;
    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            float u = x / static_cast<float>(width);
            float v = y / static_cast<float>(height);

            ray.Origin = p0 + u * right + v * down + E;
            ray.Direction = glm::normalize(ray.Origin - E);

            const glm::vec3 color = Sample(ray);

            m_ScreenData[colorIndex] = color.x;
            m_ScreenData[colorIndex + 1] = color.y;
            m_ScreenData[colorIndex + 2] = color.z;

            colorIndex += stride;
        }
    }

    if (!m_Hit)
    {
        LOG_INFO("No hits!");
    }

    GetViewport()->BindTexture();
    GetViewport()->SetDataF(&m_ScreenData[0], size);

    Muse::Renderer2D::EndScene();
}

void GamePT::OnImGuiRender()
{
    ImGui::Begin("Info");

    ImGui::Text("FPS: %f", 1 / GetDeltaTime());

    std::shared_ptr<Muse::Scene> scene = Muse::SceneManager::GetActiveScene();

    const int raysSend = scene->GetRaysSend();
    const int raysHit = scene->GetRaysHit();

    float raysHitRate = 0;
    if (raysSend != 0 && raysHit != 0)
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

void GamePT::Resize(unsigned a_Width, unsigned a_Height)
{
    m_ScreenData.clear();
    m_ScreenData.resize(a_Height * a_Width * 4);

    m_Height = a_Height;
    m_Width = a_Width;
}

glm::vec3 GamePT::Sample(const Muse::Ray& a_Ray)
{
    if(!a_Ray.Cast(m_RayHitData))
    {
        return m_BackgroundColor;
    }

    if (m_RayHitData.m_RenderComponent->GetisLight()) return m_RayHitData.m_RenderComponent->GetLightColor();

    m_Hit = true;

    m_GetColorParameters.RayDirection = a_Ray.Direction;
    m_GetColorParameters.Bounces = 5;

    glm::vec3 intersectionPoint = m_RayHitData.UpdateIntersectionPoint(a_Ray);
    glm::vec3 normal = m_RayHitData.m_RenderComponent->GetNormal(intersectionPoint);
    glm::vec3 diffuseReflection = normal + AddNoiseOnAngle(0, 180);

    Muse::Ray newRay{ intersectionPoint, diffuseReflection };

    glm::vec3 brdf = m_RayHitData.m_RenderComponent->GetColor() / glm::pi<float>();

    glm::vec3 ei = Sample(newRay) * glm::dot(normal, diffuseReflection);
    glm::vec3 result = glm::pi<float>() * 2.0f * brdf * ei;
    return result;
}

glm::vec3 GamePT::AddNoiseOnAngle(float a_Min, float a_Max)
{
    // Find random angle between min & max inclusive
    float xNoise = Random(a_Min, a_Max);
    float yNoise = Random(a_Min, a_Max);
    float zNoise = Random(a_Min, a_Max);

    // Convert Angle to Vector3
    glm::vec3 noise = glm::vec3(
        glm::sin(2 * glm::pi<float>() * xNoise / 360),
        glm::sin(2 * glm::pi<float>() * yNoise / 360),
        glm::sin(2 * glm::pi<float>() * zNoise / 360)
    );
    return noise;
}

float GamePT::Random(float a_Min, float a_Max)
{
    float range = a_Max - a_Min;

    return range * GamePT::Random() + a_Min;
}