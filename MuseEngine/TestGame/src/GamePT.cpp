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



    glm::mat4 newCameraTransform = camera->GetTransform()->GetModelMatrix();

    if(newCameraTransform != cameraTransform)
    {
        m_Buffer.clear();
        m_Buffer.resize(height * width * 4);
        m_Frames = 0;
    }
    cameraTransform = newCameraTransform;
    m_Frames++;

    glm::vec3 p0 = cameraTransform * glm::vec4(-1, -1, 1, 0); // top-left
    glm::vec3 p1 = cameraTransform * glm::vec4(1, -1, 1, 0); // top-right
    glm::vec3 p2 = cameraTransform * glm::vec4(-1, 1, 1, 0); // bottom-left

    glm::vec3 E = cameraTransform * glm::vec4(0, 0, 0, 1);
    glm::vec3 right = p1 - p0;
    glm::vec3 down = p2 - p0;

    Muse::Ray ray;

    int colorIndex = 0;
    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            float u = x / static_cast<float>(width);
            float v = y / static_cast<float>(height);

            ray.Origin = p0 + u * right + v * down + E;
            ray.Direction = glm::normalize(ray.Origin - E);

            m_FrameRayCount = 0;

            //const glm::vec3 color = Sample(ray);
            const glm::vec3 color = SampleNEE(ray);

            m_Buffer[colorIndex] += color.x;
            m_Buffer[colorIndex + 1] += color.y;
            m_Buffer[colorIndex + 2] += color.z;

            m_ScreenData[colorIndex] = m_Buffer[colorIndex] / m_Frames;
            m_ScreenData[colorIndex + 1] = m_Buffer[colorIndex + 1] / m_Frames;
            m_ScreenData[colorIndex + 2] = m_Buffer[colorIndex + 2] / m_Frames;

            colorIndex += stride;
        }
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

    m_Buffer.clear();
    m_Buffer.resize(a_Height * a_Width * 4);

    m_Height = a_Height;
    m_Width = a_Width;
}

glm::vec3 GamePT::Sample(const Muse::Ray& a_Ray)
{
    m_FrameRayCount++;

    if(!a_Ray.Cast(m_RayHitData) || m_FrameRayCount >= m_FrameRayMax)
    {
        return m_BackgroundColor;
    }

    if (m_RayHitData.m_RenderComponent->GetisLight()) return m_RayHitData.m_RenderComponent->GetLightColor();

    m_Hit = true;

    m_GetColorParameters.RayDirection = a_Ray.Direction;
    m_GetColorParameters.Bounces = 5;

    glm::vec3 intersectionPoint = m_RayHitData.UpdateIntersectionPoint(a_Ray);
    glm::vec3 normal = m_RayHitData.m_RenderComponent->GetNormal(intersectionPoint);
    glm::vec3 diffuseReflection = RandomDirectionInHemisphere(normal);

    Muse::Ray newRay{ intersectionPoint, diffuseReflection };

    glm::vec3 brdf = m_RayHitData.m_RenderComponent->GetColor() / glm::pi<float>();

    glm::vec3 ei = Sample(newRay) * glm::dot(normal, diffuseReflection);
    glm::vec3 result = glm::pi<float>() * 2.0f * brdf * ei;
    return result;
}

glm::vec3 GamePT::SampleNEE(const Muse::Ray& a_Ray)
{
    m_FrameRayCount++;

    if (!a_Ray.Cast(m_RayHitData) || m_FrameRayCount >= m_FrameRayMax)
    {
        return m_BackgroundColor;
    }

    if(m_RayHitData.m_RenderComponent->GetisLight())
    {
        return m_BackgroundColor;
    }


    glm::vec3 materialColor = m_RayHitData.m_RenderComponent->GetColor();

    //Russian Roulette
    float surivalRate = std::max(std::max(materialColor.x, materialColor.y), materialColor.z);

    if (Random() > surivalRate)
    {
        return m_BackgroundColor;
    }

    glm::vec3 brdf = m_RayHitData.m_RenderComponent->GetColor() / glm::pi<float>();
    brdf /= surivalRate;

    //NEE
    glm::vec3 intersectionPoint = m_RayHitData.UpdateIntersectionPoint(a_Ray);
    float area = 2.f;
    glm::vec3 lightPosition = glm::vec3(0, 5, 0);
    glm::vec3 directionToLight = lightPosition - intersectionPoint;
    float distanceToLight = glm::distance(lightPosition, intersectionPoint);
    glm::vec3 lightNormal = glm::vec3(0, -1, 0);
    Muse::Ray lightRay{ intersectionPoint, directionToLight };
    glm::vec3 light = glm::vec3(0);
    glm::vec3 lightColor = glm::vec3(1);

    glm::vec3 normal = m_RayHitData.m_RenderComponent->GetNormal(intersectionPoint);

    if(glm::dot(normal, directionToLight) > 0 &&
        glm::dot(lightNormal, -directionToLight))
    {
        if(lightRay.Cast(distanceToLight))
        {
            float solidAngle = (glm::dot(lightNormal, -directionToLight) * area) / (distanceToLight * distanceToLight);

            light = lightColor * solidAngle * brdf * glm::dot(normal, directionToLight);
        }
    }

    glm::vec3 diffuseReflection = RandomDirectionInHemisphere(normal);
    //glm::vec3 diffuseReflection = TransformToTangent(normal, CosineWeightedDiffuseReflection());
    //glm::vec3 diffuseReflection = normal * CosineWeightedDiffuseReflection();

    Muse::Ray newRay{ intersectionPoint, diffuseReflection };

    float pdf = glm::dot(normal, diffuseReflection) / glm::pi<float>();

    //glm::vec3 ei = Sample(newRay) * glm::dot(normal, diffuseReflection) / pdf;
    glm::vec3 ei = Sample(newRay) * glm::dot(normal, diffuseReflection);

    glm::vec3 result = glm::pi<float>() * 2.0f * brdf * ei + light;
    return result;
}

glm::vec3 GamePT::RandomDirectionInHemisphere(const glm::vec3& a_Normal)
{
    glm::vec3 hemisphere;

    float azimuth = Random() * glm::pi<float>() * 2.0f;
    hemisphere.y = Random();

    float sin_elevation = sqrtf(1 - hemisphere.y * hemisphere.y);
    hemisphere.x = sin_elevation * cos(azimuth);
    hemisphere.z = sin_elevation * sin(azimuth);

    glm::vec3 Nt;
    glm::vec3 Nb;

    if (fabs(a_Normal.x) > fabs(a_Normal.y))
    {
        Nt = glm::normalize(glm::vec3(a_Normal.z, 0, -a_Normal.x));
    }
    else
    {
        Nt = glm::normalize(glm::vec3(0, -a_Normal.z, a_Normal.y));
    }
    Nb = glm::cross( a_Normal, Nt);

    glm::vec3 randomDirection(
        hemisphere.x * Nb.x + hemisphere.y * a_Normal.x + hemisphere.z * Nt.x
        , hemisphere.x * Nb.y + hemisphere.y * a_Normal.y + hemisphere.z * Nt.y
        , hemisphere.x * Nb.z + hemisphere.y * a_Normal.z + hemisphere.z * Nt.z);

    return randomDirection;
}

float GamePT::Random(float a_Min, float a_Max)
{
    float range = a_Max - a_Min;

    return range * GamePT::Random() + a_Min;
}

/*
glm::vec3 GamePT::CosineWeightedDiffuseReflection(const glm::vec3& origin) const
{
    glm::vec3 Nt, Nb;
    rng.createCoordinateSystem(normal, Nt, Nb);
    const float r1 = rng.Rand(1.0f);
    const float r2 = rng.Rand(1.0f);
    const vec3 sample = cosineWeightedSample(r1, r2);
    const vec3 dir = rng.localToWorld(sample, Nt, Nb, normal);
    return { origin + EPSILON * dir, dir };
}
*/