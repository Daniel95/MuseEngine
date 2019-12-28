#pragma once

#include "Core/Application.h"

namespace Muse
{
    class OrthographicCamera;
    class Shader;
    class VertexBuffer;
    class IndexBuffer;
    class VertexArray;
}

class GameApplication : public Muse::Application
{
public:
    GameApplication() = default;
    virtual ~GameApplication() = default;

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float a_DeltaTime) override;
    virtual void OnFixedUpdate() override;
    virtual void OnRender() override;

    virtual void OnWindowCloseEvent();
    virtual void OnWindowResizeEvent(int a_Width, int a_Height);
    virtual void OnKeyPressedEvent(int a_KeyCode, int a_RepeatCount);
    virtual void OnKeyReleasedEvent(int a_KeyCode);
    virtual void OnMouseButtonPressedEvent(int a_Button);
    virtual void OnMouseButtonReleasedEvent(int a_Button);
    virtual void OnMouseScrolledEvent(float a_XOffset, float a_YOffset);
    virtual void OnMouseMovedEvent(float a_X, float a_Y);

private:
    std::shared_ptr<Muse::Shader> m_Shader;
    std::shared_ptr<Muse::Shader> m_BlueShader;
    std::shared_ptr<Muse::VertexBuffer> m_VB;
    std::shared_ptr<Muse::IndexBuffer> m_IB;
    std::shared_ptr<Muse::VertexArray> m_TriangleVA;
    std::shared_ptr<Muse::VertexArray> m_SquareVA;
    std::shared_ptr<Muse::OrthographicCamera> m_Camera;

};
