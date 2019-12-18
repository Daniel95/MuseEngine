#pragma once

#include "Layer.h"

namespace Muse
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(Timestep ts) override;
        //virtual void OnEvent(EventOld& event) override;

    private:
        float m_Time;

        void OnMouseButtonPressedEvent(int a_Button);
        void OnMouseButtonReleasedEvent(int a_Button);
        void OnMouseMovedEvent(float a_X, float a_Y);
        void OnMouseScrolledEvent(float a_XOffset, float a_YOffset);
        void KeyPressedEvent(int a_KeyCode, int a_RepeatCount);
        void OnKeyReleasedEvent(int a_KeyCode);
        void OnKeyTyped(int a_KeyCode);
        void OnWindowResize(int a_Width, int a_Height);
    };
}
