#pragma once

#include "Core/Application.h"
#include "glm/vec4.hpp"

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
    static float Random() { return static_cast<double>(rand()) / RAND_MAX + 1; }

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float a_DeltaTime) override;
    virtual void OnFixedUpdate() override;
    virtual void OnRender() override;
    virtual void OnImGuiRender() override;

private:
    std::vector<float> m_ScreenData;
    unsigned int m_Width, m_Height;
    glm::vec3 m_BackgroundColor = glm::vec3(1);
    Muse::RayHitData m_RayHitData;
    Muse::GetColorParameters m_GetColorParameters;
    bool m_Hit = false;

    void Resize(unsigned int a_Width, unsigned int a_Height);
    glm::vec3 Sample(const Muse::Ray& a_Ray);
    static glm::vec3 AddNoiseOnAngle(float a_Min, float a_Max);


};
