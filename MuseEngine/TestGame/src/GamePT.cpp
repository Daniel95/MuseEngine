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
#include "Core/Renderer/RayTracing/Shape/Shape.h"

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

    auto renderComponents = Muse::RenderComponent::GetAll();

    for(auto renderComponent : renderComponents)
    {
        if(renderComponent->IsLight())
        {
            m_Light = renderComponent;
            break;
        }
    }




    glm::vec3 direction(0.5f, 0.5f, 0);
    glm::vec3 normal(-0.3f, 0.6f, 0);


    glm::vec3 difference = direction - (normal * -1.f);
    glm::vec3 result = direction + difference;


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
                color = SampleIS(ray);

            }
            else
            {
                color = Sample(ray);
                //color = SampleNEE(ray, true);
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

    glm::vec3 materialColor = m_RayHitData.m_RenderComponent->GetColor();
    glm::vec3 brdf = m_RayHitData.m_RenderComponent->GetColor() / glm::pi<float>();

    //Russian Roulette
    float surivalRate = std::clamp(std::max(std::max(materialColor.x, materialColor.y), materialColor.z), 0.1f, 0.9f);

    if (Muse::Random() > surivalRate)
    {
        return m_BackgroundColor;
    }

    brdf /= surivalRate;

    m_Hit = true;

    m_GetColorParameters.RayDirection = a_Ray.Direction;
    m_GetColorParameters.Bounces = 5;

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

    if (m_RayHitData.m_RenderComponent->IsLight())
    {
        return m_RayHitData.m_RenderComponent->GetLightColor();
    }

    glm::vec3 materialColor = m_RayHitData.m_RenderComponent->GetColor();
    glm::vec3 brdf = m_RayHitData.m_RenderComponent->GetColor() / glm::pi<float>();

    //Russian Roulette
    float surivalRate = std::clamp(std::max(std::max(materialColor.x, materialColor.y), materialColor.z), 0.1f, 0.9f);

    if (Muse::Random() > surivalRate)
    {
        return m_BackgroundColor;
    }

    brdf /= surivalRate;

    m_Hit = true;

    m_GetColorParameters.RayDirection = a_Ray.Direction;
    m_GetColorParameters.Bounces = 5;

    glm::vec3 intersectionPoint = m_RayHitData.UpdateIntersectionPoint(a_Ray);
    glm::vec3 normal = m_RayHitData.m_RenderComponent->GetNormal(intersectionPoint);
    //glm::vec3 diffuseReflection = Muse::RandomDirectionInHemisphere(normal);
    glm::vec3 diffuseReflection = CosineWeightedDiffuseReflection();
    //glm::vec3 diffuseReflection = Muse::RandomDirectionInHemisphere(normal);


    glm::mat3 normalRotationMatrix = MakeRotationMat(normal);


    glm::vec3 transformedDiffuseReflection = normalRotationMatrix * diffuseReflection;

    Muse::Ray newRay{ intersectionPoint, transformedDiffuseReflection };

    glm::vec3 ei = Sample(newRay) * glm::dot(normal, transformedDiffuseReflection);
    glm::vec3 result = glm::pi<float>() * 2.0f * brdf * ei;
    return result;
}

/*
glm::vec3 GamePT::SampleIS(const Muse::Ray& a_Ray)
{
    m_FrameRayCount++;

    if (!a_Ray.Cast(m_RayHitData) || m_FrameRayCount >= m_FrameRayMax)
    {
        return m_BackgroundColor;
    }

    if (m_RayHitData.m_RenderComponent->IsLight())
    {
        return m_RayHitData.m_RenderComponent->GetLightColor();
    }

    glm::vec3 materialColor = m_RayHitData.m_RenderComponent->GetColor();
    glm::vec3 brdf = m_RayHitData.m_RenderComponent->GetColor() / glm::pi<float>();

    //Russian Roulette
    float surivalRate = std::clamp(std::max(std::max(materialColor.x, materialColor.y), materialColor.z), 0.1f, 0.9f);

    if (Muse::Random() > surivalRate)
    {
        return m_BackgroundColor;
    }

    brdf /= surivalRate;

    m_Hit = true;

    m_GetColorParameters.RayDirection = a_Ray.Direction;
    m_GetColorParameters.Bounces = 5;

    glm::vec3 intersectionPoint = m_RayHitData.UpdateIntersectionPoint(a_Ray);
    glm::vec3 normal = m_RayHitData.m_RenderComponent->GetNormal(intersectionPoint);


    glm::vec3 diffuseReflection = Muse::RandomDirectionInHemisphere(glm::vec3(0, 0, 1));
    //glm::vec3 diffuseReflection = m_RayHitData.m_RenderComponent->GetTransform()->TransformVector(CosineWeightedDiffuseReflection());
    //glm::vec3 diffuseReflection = normal + CosineWeightedDiffuseReflection();
    //glm::vec3 diffuseReflection = glm::normalize(getBRDFRay(intersectionPoint, normal, a_Ray.Direction));
    //glm::vec3 cosineWeightedDiffuseReflection = CosineWeightedDiffuseReflection();
    //glm::vec3 diffuseReflection = TransformToDirection(normal, cosineWeightedDiffuseReflection);


    glm::mat3 normalRotationMatrix = MakeRotationMat(normal);

    glm::vec3 transformedDiffuseReflection = normalRotationMatrix * diffuseReflection;

    float pdf = glm::dot(normal, transformedDiffuseReflection);

    Muse::Ray newRay{ intersectionPoint, diffuseReflection };

    if(normal != glm::vec3(0, 1, 0))
    {
        //LOG_ENGINE_INFO("yo");
    }


    glm::vec3 ei = SampleIS(newRay) * glm::dot(normal, diffuseReflection);
    glm::vec3 result = glm::pi<float>() * 2.0f * brdf * ei;
    return result;
}
*/

glm::vec3 GamePT::SampleNEE(const Muse::Ray& a_Ray, bool a_LastSpecular)
{
    m_FrameRayCount++;

    if (!a_Ray.Cast(m_RayHitData) || m_FrameRayCount >= m_FrameRayMax)
    {
        return m_BackgroundColor;
    }

    glm::vec3 brdf = m_RayHitData.m_RenderComponent->GetColor() / glm::pi<float>();

    if(m_RayHitData.m_RenderComponent->IsLight())
    {
        if(a_LastSpecular)
        {
            return m_RayHitData.m_RenderComponent->GetLightColor();
        }
        else
        {
            return m_RayHitData.m_RenderComponent->GetLightColor();
        }
    }

    glm::vec3 intersectionPoint = m_RayHitData.UpdateIntersectionPoint(a_Ray);

    glm::vec3 lightPosition = m_Light->GetShape()->GetRandomPointInShape();
    glm::vec3 scale = m_Light->GetTransform()->GetScale();

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





    glm::vec3 materialColor = m_RayHitData.m_RenderComponent->GetColor();

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

    glm::vec3 brdf = m_RayHitData.m_RenderComponent->GetColor() / glm::pi<float>();

    if (m_RayHitData.m_RenderComponent->IsLight())
    {
        if (a_LastSpecular)
        {
            return m_RayHitData.m_RenderComponent->GetLightColor();
        }
        else
        {
            return m_BackgroundColor;
        }
    }

    glm::vec3 intersectionPoint = m_RayHitData.UpdateIntersectionPoint(a_Ray);

    glm::vec3 lightPosition = m_Light->GetShape()->GetRandomPointInShape();
    glm::vec3 scale = m_Light->GetTransform()->GetScale();

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





    glm::vec3 materialColor = m_RayHitData.m_RenderComponent->GetColor();

    //Russian Roulette
    float surivalRate = std::max(std::max(materialColor.x, materialColor.y), materialColor.z);

    if (Muse::Random() > surivalRate)
    {
        return light;
    }

    brdf /= surivalRate;


    //glm::vec3 diffuseReflection = Muse::RandomDirectionInHemisphere(normal);
    //glm::vec3 diffuseReflection = m_RayHitData.m_RenderComponent->GetTransform()->InverseTransformVector(CosineWeightedDiffuseReflection());
    glm::vec3 diffuseReflection = getBRDFRay(intersectionPoint, normal, a_Ray.Direction);



    Muse::Ray newRay{ intersectionPoint, diffuseReflection };

    float pdf = glm::dot(normal, diffuseReflection) / glm::pi<float>();
    //float pdf = 1 / (2 * glm::pi<float>());

    //float angleOffset = glm::dot(normal, diffuseReflection);
    //float angleOffsetPDF = glm::dot(normal, diffuseReflection) / pdf;
    //glm::vec3 sample = SampleNEE(newRay, false);

    //glm::vec3 ei = SampleNEE(newRay, false) * glm::dot(normal, diffuseReflection);
    glm::vec3 eiPDF = SampleNEE(newRay, false) * glm::dot(normal, diffuseReflection) / pdf;
    //glm::vec3 ei = SampleNEE(newRay, false) * glm::dot(normal, diffuseReflection);

    glm::vec3 result = glm::pi<float>() * 2.0f * brdf * eiPDF + light;
    return result;
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