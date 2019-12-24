#pragma once

namespace Muse
{
    class Input
    {
    public:
        virtual ~Input() = default;
        static bool IsKeyPressed(int a_Keycode) { return s_Instance->IsKeyPressedImpl(a_Keycode); }
        static bool IsMouseButtonPressed(int a_Button) { return s_Instance->IsMouseButtonPressedImpl(a_Button); }
        static glm::vec2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
        static bool GetMouseX() { return s_Instance->GetMouseXImpl(); }
        static bool GetMouseY() { return s_Instance->GetMouseYImpl(); }

    protected:
        virtual bool IsKeyPressedImpl(int a_Keycode) const = 0;
        virtual bool IsMouseButtonPressedImpl(int a_Button) const = 0;
        virtual glm::vec2 GetMousePositionImpl() const = 0;
        virtual float GetMouseXImpl() const = 0;
        virtual float GetMouseYImpl() const = 0;

    private:
        static Input* s_Instance;

    };
}
