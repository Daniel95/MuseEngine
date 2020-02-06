#pragma once

#include "Core/Application.h"
#include "glm/glm.hpp"

#include <vector>
#include <array>
#include "Core/Renderer/RayTracing/RayHitData.h"
#include "Core/Renderer/RayTracing/GetColorParameters.h"

namespace Muse
{
    struct Ray;
    class Texture;
    class Shader;
}

class GamePT : public Muse::Application
{
public:
    GamePT() = default;
    virtual ~GamePT() = default;

    static float Random(float a_Min, float a_Max);
    static float Random() { return static_cast<float>(rand()) / RAND_MAX; }

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float a_DeltaTime) override;
    virtual void OnFixedUpdate() override;
    virtual void OnRender() override;
    virtual void OnImGuiRender() override;

private:
    std::vector<float> m_ScreenData;
    std::vector<float> m_Buffer;
    unsigned int m_Width, m_Height;
    glm::vec3 m_BackgroundColor = glm::vec3(0);
    Muse::RayHitData m_RayHitData;
    Muse::GetColorParameters m_GetColorParameters;
    bool m_Hit = false;
    int m_Frames = 0;
    const int m_FrameRayMax = 5;
    int m_FrameRayCount = 0;
    glm::mat4 cameraTransform;

    void Resize(unsigned int a_Width, unsigned int a_Height);
    glm::vec3 Sample(const Muse::Ray& a_Ray);
    glm::vec3 SampleNEE(const Muse::Ray& a_Ray);
    static glm::vec3 RandomDirectionInHemisphere(const glm::vec3& a_Normal);

    glm::vec3 CosineWeightedDiffuseReflection() const
    {
        float r0 = Random(), r1 = Random();
        float r = sqrt(r0);
        float theta = 2 * PI * r1;
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        return glm::vec3(x, y, sqrt(1 - r0));
    }

    glm::vec3 TransformToTangent(const glm::vec3& normal, const glm::vec3& vector) const
    {
        const glm::vec3 w = abs(normal.x > 0.99f) ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
        const glm::vec3 t = normalize(cross(normal, w));
        const glm::vec3 b = cross(t, normal);
        return (vector * t, vector * b, vector * normal);
    }

};
