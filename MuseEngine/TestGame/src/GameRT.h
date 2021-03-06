﻿#pragma once

#include "Core/Application.h"
#include "glm/vec4.hpp"

#include <vector>
#include <array>

namespace Muse
{
    class RenderComponent;
    class PerspectiveCamera;
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
    std::vector<float> m_ScreenData;

    unsigned int m_Width, m_Height;

    void Resize(unsigned int a_Width, unsigned int a_Height);
};
