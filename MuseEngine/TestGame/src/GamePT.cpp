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
#include "Core/Renderer/RayTracer/RayHitData.h"
#include "Core/Renderer/RayTracer/GetColorParameters.h"
#include "Core/Renderer/RayTracer/BVH/BVH.h"
#include "Core/Renderer/RayTracer/Ray.h"
#include "RayTracer/SceneLibraryRT.h"
#include "Core/Gameplay/Component/CameraComponent.h"
#include "Core/Renderer/RayTracer/Shape/Shape.h"
#include "Core/Renderer/RayTracer/Ray.h"
#include "Core/Renderer/RayTracer/RayHitData.h"
#include "Core/Renderer/RayTracer/GetColorParameters.h"
#include "Core/Renderer/RayTracer/RendererPT.h"

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

    std::shared_ptr <Muse::Scene> scene = Muse::Scene::Create();
    Muse::ResourceManager::Add("GamePTScene", scene);
    Muse::SceneManager::SwitchScene(scene);

    m_Height = GetViewport()->GetHeight();
    m_Width = GetViewport()->GetHeight();

    m_ScreenData.resize(m_Height * m_Width * 4);

    //scene->ConstructBVH();

    SceneLibraryRT::MakePTBenchmarkScene(scene);

    auto renderComponents = Muse::RenderComponent::GetAll();

    for(auto renderComponent : renderComponents)
    {
        if(renderComponent->GetMaterial().MaterialType == Muse::MaterialType::Light)
        {
            m_LightsRenderComponents.push_back(renderComponent);
        }
    }



    /*
    //glm::vec3 result = TransformToDirection(normal, direction);
    glm::vec3 tangent = find_orthogonal_unit_vector(normal);
    glm::vec3 bitangent = glm::cross(tangent, normal);

    glm::vec3 result = direction.x * tangent + direction.y * direction + direction.z * bitangent;
    */



    //glm::vec3 result = ToTangent(normal, direction);


    //LOG_ENGINE_INFO("{0}", result.x);






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

    Muse::CameraComponent* camera = Muse::CameraComponent::GetMain();

    Muse::Renderer2D::BeginScene(*camera);

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

    glm::mat4 newCameraTransform = camera->GetTransform()->GetWorldModelMatrix();

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

    const int halfWidth = width * 0.5f;

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

            glm::vec3 color;

            if(x <= halfWidth)
            {
                //color = SampleNew(ray);
                //color = SampleNEEIS(ray, true);
                color = SampleIS(ray);

            }
            else
            {
                //color = SampleNEEIS(ray, true);
                //color = SampleNEE(ray, true);
                //color = SampleIS(ray);
                color = SampleNEEIS(ray, true);

                //color = Sample(ray);
            }

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

glm::vec3 GamePT::SampleNew(const Muse::Ray& a_Ray)
{
    m_FrameRayCount++;

    if (!a_Ray.Cast(m_RayHitData) || m_FrameRayCount >= m_FrameRayMax)
    {
        return m_BackgroundColor;
    }

    const Muse::Material& material = m_RayHitData.m_RenderComponent->GetMaterial();
    const glm::vec3 intersectionPoint = m_RayHitData.UpdateIntersectionPoint(a_Ray);

    bool continueSampling;

    glm::vec3 color = Muse::RendererPT::CalculateColor(material, intersectionPoint, continueSampling);

    if(!continueSampling)
    {
        return color;
    }

    glm::vec3 brdf = color / glm::pi<float>();

    //Russian Roulette
    float surivalRate = std::clamp(std::max(std::max(color.x, color.y), color.z), 0.1f, 0.9f);

    if (Muse::Random() > surivalRate)
    {
        return m_BackgroundColor;
    }

    brdf /= surivalRate;

    m_Hit = true;

    glm::vec3 normal = m_RayHitData.m_RenderComponent->GetNormal(intersectionPoint);
    glm::vec3 diffuseReflection = Muse::RandomDirectionInHemisphere(normal);

    Muse::Ray newRay{ intersectionPoint, diffuseReflection };


    glm::vec3 ei = Sample(newRay) * glm::dot(normal, diffuseReflection);
    glm::vec3 result = glm::pi<float>() * 2.0f * brdf * ei;
    return result;
}

glm::vec3 GamePT::Sample(const Muse::Ray& a_Ray)
{
    m_FrameRayCount++;

    if(!a_Ray.Cast(m_RayHitData) || m_FrameRayCount >= m_FrameRayMax)
    {
        return m_BackgroundColor;
    }

    if (m_RayHitData.m_RenderComponent->IsLight())
    {
        return m_RayHitData.m_RenderComponent->GetLightColor();
    }

    glm::vec3 materialColor = m_RayHitData.m_RenderComponent->GetMaterial().Color;
    glm::vec3 brdf = materialColor / glm::pi<float>();

    //Russian Roulette
    float surivalRate = std::clamp(std::max(std::max(materialColor.x, materialColor.y), materialColor.z), 0.1f, 0.9f);

    if (Muse::Random() > surivalRate)
    {
        return m_BackgroundColor;
    }

    brdf /= surivalRate;

    m_Hit = true;

    //m_GetColorParameters.Ray->Direction = a_Ray.Direction;
    //m_GetColorParameters.Bounces = 5;

    glm::vec3 intersectionPoint = m_RayHitData.UpdateIntersectionPoint(a_Ray);
    glm::vec3 normal = m_RayHitData.m_RenderComponent->GetNormal(intersectionPoint);
    glm::vec3 diffuseReflection = Muse::RandomDirectionInHemisphere(normal);

    Muse::Ray newRay{ intersectionPoint, diffuseReflection };


    glm::vec3 ei = Sample(newRay) * glm::dot(normal, diffuseReflection);
    glm::vec3 result = glm::pi<float>() * 2.0f * brdf * ei;
    return result;
}

glm::vec3 GamePT::SampleIS(const Muse::Ray& a_Ray)
{
    m_FrameRayCount++;

    if (!a_Ray.Cast(m_RayHitData) || m_FrameRayCount >= m_FrameRayMax)
    {
        return m_BackgroundColor;
    }

    const Muse::Material& material = m_RayHitData.m_RenderComponent->GetMaterial();
    const glm::vec3 intersectionPoint = m_RayHitData.UpdateIntersectionPoint(a_Ray);

    bool continueSampling;

    glm::vec3 color = Muse::RendererPT::CalculateColor(material, intersectionPoint, continueSampling);

    if (!continueSampling)
    {
        return color;
    }

    glm::vec3 brdf = color / glm::pi<float>();

    //Russian Roulette
    float surivalRate = std::clamp(std::max(std::max(color.x, color.y), color.z), 0.1f, 0.9f);

    if (Muse::Random() > surivalRate)
    {
        return m_BackgroundColor;
    }

    brdf /= surivalRate;

    glm::vec3 normal = m_RayHitData.m_RenderComponent->GetNormal(intersectionPoint);
    glm::vec3 diffuseReflection = CosineWeightedDiffuseReflection();

    glm::mat3 normalRotationMatrix = MakeRotationMat(normal);

    glm::vec3 transformedDiffuseReflection = normalRotationMatrix * diffuseReflection;

    Muse::Ray newRay{ intersectionPoint, transformedDiffuseReflection };

    glm::vec3 ei = SampleIS(newRay) * glm::dot(normal, transformedDiffuseReflection);
    glm::vec3 result = glm::pi<float>() * 2.0f * brdf * ei;
    return result;
}

glm::vec3 GamePT::SampleNEE(const Muse::Ray& a_Ray, bool a_LastSpecular)
{
    m_FrameRayCount++;

    if (!a_Ray.Cast(m_RayHitData) || m_FrameRayCount >= m_FrameRayMax)
    {
        return m_BackgroundColor;
    }

    const Muse::Material& material = m_RayHitData.m_RenderComponent->GetMaterial();
    const glm::vec3 intersectionPoint = m_RayHitData.UpdateIntersectionPoint(a_Ray);

    bool hitLight;

    glm::vec3 color = Muse::RendererPT::CalculateColor(material, intersectionPoint, hitLight);

    if (!hitLight)
    {
        if (a_LastSpecular)
        {
            return color;
        }
        else
        {
            return m_BackgroundColor;
        }
    }

    glm::vec3 brdf = color / glm::pi<float>();

    int randomLightIndex = Muse::Random() * (m_LightsRenderComponents.size() - 1);
    std::shared_ptr<Muse::RenderComponent> lightRenderComponent = m_LightsRenderComponents[randomLightIndex];
    glm::vec3 lightPosition = lightRenderComponent->GetShape()->GetRandomPointInShape();
    glm::vec3 scale = lightRenderComponent->GetTransform()->GetScale();

    //NEE
    float area = scale.x * scale.z;
    //float area = 1;
    glm::vec3 directionToLight = glm::normalize(lightPosition - intersectionPoint);
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

    glm::vec3 materialColor = m_RayHitData.m_RenderComponent->GetMaterial().Color;

    //Russian Roulette
    float surivalRate = std::max(std::max(materialColor.x, materialColor.y), materialColor.z);

    if (Muse::Random() > surivalRate)
    {
        return light;
    }

    brdf /= surivalRate;

    glm::vec3 diffuseReflection = Muse::RandomDirectionInHemisphere(normal);

    Muse::Ray newRay{ intersectionPoint, diffuseReflection };

    glm::vec3 ei = SampleNEE(newRay, false) * glm::dot(normal, diffuseReflection);

    glm::vec3 result = glm::pi<float>() * 2.0f * brdf * ei + light;
    return result;
}


glm::vec3 GamePT::SampleNEEIS(const Muse::Ray& a_Ray, bool a_LastSpecular)
{
    m_FrameRayCount++;

    if (!a_Ray.Cast(m_RayHitData) || m_FrameRayCount >= m_FrameRayMax)
    {
        return m_BackgroundColor;
    }

    const Muse::Material& material = m_RayHitData.m_RenderComponent->GetMaterial();
    const glm::vec3 intersectionPoint = m_RayHitData.UpdateIntersectionPoint(a_Ray);

    bool hitLight;

    glm::vec3 color = Muse::RendererPT::CalculateColor(material, intersectionPoint, hitLight);

    if (!hitLight)
    {
        if (a_LastSpecular)
        {
            return color;
        }
        else
        {
            return m_BackgroundColor;
        }
    }

    glm::vec3 brdf = color / glm::pi<float>();

    int randomLightIndex = Muse::Random() * (m_LightsRenderComponents.size() - 1);
    std::shared_ptr<Muse::RenderComponent> lightRenderComponent = m_LightsRenderComponents[randomLightIndex];
    glm::vec3 lightPosition = lightRenderComponent->GetShape()->GetRandomPointInShape();
    glm::vec3 scale = lightRenderComponent->GetTransform()->GetScale();

    //NEE
    float area = scale.x * scale.z;
    //float area = 1;
    glm::vec3 directionToLight = glm::normalize(lightPosition - intersectionPoint);
    float distanceToLight = glm::distance(lightPosition, intersectionPoint);
    glm::vec3 lightNormal = glm::vec3(0, -1, 0);
    Muse::Ray lightRay{ intersectionPoint, directionToLight };
    glm::vec3 light = glm::vec3(0);
    glm::vec3 lightColor = glm::vec3(1);

    glm::vec3 normal = m_RayHitData.m_RenderComponent->GetNormal(intersectionPoint);

    if (glm::dot(normal, directionToLight) > 0 &&
        glm::dot(lightNormal, -directionToLight))
    {
        if (lightRay.Cast(distanceToLight))
        {
            float solidAngle = (glm::dot(lightNormal, -directionToLight) * area) / (distanceToLight * distanceToLight);

            light = lightColor * solidAngle * brdf * glm::dot(normal, directionToLight);
        }
    }






    glm::vec3 materialColor = m_RayHitData.m_RenderComponent->GetMaterial().Color;

    //Russian Roulette
    float surivalRate = std::max(std::max(materialColor.x, materialColor.y), materialColor.z);

    if (Muse::Random() > surivalRate)
    {
        return light;
    }

    brdf /= surivalRate;


    glm::vec3 diffuseReflection = CosineWeightedDiffuseReflection();


    glm::mat3 normalRotationMatrix = MakeRotationMat(normal);


    glm::vec3 transformedDiffuseReflection = normalRotationMatrix * diffuseReflection;

    Muse::Ray newRay{ intersectionPoint, transformedDiffuseReflection };

    glm::vec3 ei = SampleNEEIS(newRay, false) * glm::dot(normal, transformedDiffuseReflection);
    glm::vec3 result = glm::pi<float>() * 2.0f * brdf * ei;
    return result;
}