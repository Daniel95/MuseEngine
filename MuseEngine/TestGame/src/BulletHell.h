#pragma once

#include "Core/Application.h"
#include "glm/vec3.hpp"

#include <vector>

namespace Muse
{
    class Texture;
    class Shader;
}

class BulletHell : public Muse::Application
{
public:
    BulletHell() = default;
    virtual ~BulletHell() = default;

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float a_DeltaTime) override {}
    virtual void OnFixedUpdate() override {}
    virtual void OnRender() override;
    virtual void OnImGuiRender() override;

private:
    std::shared_ptr<Muse::Shader> m_FlatColorShader;
    glm::vec3 m_FlatShaderColor = { 0.8, 0.2, 0.4 };
    std::shared_ptr<Muse::Texture> m_RaymanTexture;
    std::shared_ptr<Muse::Texture> m_CheckerboardTexture;

    const float m_CameraMoveSpeed = 0.3f;
    float m_GridCellRotation = 0;

};
