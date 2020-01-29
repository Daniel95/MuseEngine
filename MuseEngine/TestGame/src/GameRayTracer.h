#pragma once

#include "Core/Application.h"
#include "glm/vec4.hpp"

namespace Muse
{
    class Shader;
}

class GameRayTracer : public Muse::Application
{
public:
    GameRayTracer() = default;
    virtual ~GameRayTracer() = default;

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float a_DeltaTime) override { }
    virtual void OnFixedUpdate() override { }
    virtual void OnRender() override;
    virtual void OnImGuiRender() override;

private:
    std::shared_ptr<Muse::Shader> m_FlatColorShader;
    glm::vec4 m_FlatShaderColor = { 0.8, 0.2, 0.4, 1.0f };

    const float m_CameraMoveSpeed = 0.3f;

};
