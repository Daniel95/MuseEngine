#pragma once

#include "Core/Application.h"
#include "glm/vec3.hpp"

namespace Muse
{
    class Shader;
}

class Game2D : public Muse::Application
{
public:
    Game2D() = default;
    virtual ~Game2D() = default;

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float a_DeltaTime) override { }
    virtual void OnFixedUpdate() override { }
    virtual void OnRender() override;
    virtual void OnImGuiRender() override;

private:
    std::shared_ptr<Muse::Shader> m_FlatColorShader;
    glm::vec3 m_FlatShaderColor = { 0.8, 0.2, 0.4 };

    const float m_CameraMoveSpeed = 0.3f;

};
