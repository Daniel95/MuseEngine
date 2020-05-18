#pragma once

#include "Core/Application.h"
#include "glm/vec3.hpp"

#include <vector>
#include <memory>

namespace Muse
{
    class Texture;
    class Shader;
    class VertexBuffer;
    class VertexArray;
}

class TestGame : public Muse::Application
{
public:
    TestGame() = default;
    virtual ~TestGame() = default;

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float a_DeltaTime) override {}
    virtual void OnFixedUpdate() override {}
    virtual void OnRender() override;
    virtual void OnImGuiRender() override;

private:
    std::shared_ptr<Muse::Shader> m_Shader;
    std::shared_ptr<Muse::VertexBuffer> m_VB;
    std::shared_ptr<Muse::VertexArray> m_VA;

};
