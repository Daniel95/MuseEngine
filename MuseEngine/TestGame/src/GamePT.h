#pragma once

#include "Core/Application.h"
#include "glm/vec4.hpp"

#include <vector>
#include <array>
#include "Core/Renderer/RayTracing/RayHitData.h"

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

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float a_DeltaTime) override;
    virtual void OnFixedUpdate() override;
    virtual void OnRender() override;
    virtual void OnImGuiRender() override;

private:
    std::vector<float> m_ScreenData;
    unsigned int m_Width, m_Height;

    void Resize(unsigned int a_Width, unsigned int a_Height);
    glm::vec3 Sample(const Muse::Ray& a_Ray);

    Muse::RayHitData rayHitData;

};
