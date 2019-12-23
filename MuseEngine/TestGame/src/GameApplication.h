#pragma once

#include "Core/Application.h"
#include "Core/Utilities/Log.h"

class GameApplication : public Muse::Application
{
public:
    GameApplication() = default;
    virtual ~GameApplication() = default;

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float deltaTime) override;
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

};
