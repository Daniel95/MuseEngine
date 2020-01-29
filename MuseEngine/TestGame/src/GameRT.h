#pragma once

#include "Core/Application.h"
#include "glm/vec4.hpp"

namespace Muse
{
    class Texture;
    class Shader;
}

class GameRT : public Muse::Application
{
public:
    GameRT() = default;
    virtual ~GameRT() = default;

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float a_DeltaTime) override;
    virtual void OnFixedUpdate() override;
    virtual void OnRender() override;
    virtual void OnImGuiRender() override;

private:
    std::shared_ptr<Muse::Texture> m_ViewportTexture;

};
