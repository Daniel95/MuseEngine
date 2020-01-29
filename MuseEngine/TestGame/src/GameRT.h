#pragma once

#include "Core/Application.h"
#include "glm/vec4.hpp"

#include <vector>
#include <array>

// Display size
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

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
    std::shared_ptr<Muse::Texture> m_Test;
    std::vector<float> m_ScreenData;

    unsigned int m_Width, m_Height;

    void Resize(unsigned int a_Width, unsigned int a_Height);
};
