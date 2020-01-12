#pragma once

#include <glm/glm.hpp>

namespace Muse
{
    class Input
    {
    public:
        virtual ~Input() = default;
        static bool GetKeyDown(int a_Keycode) { return s_Instance->GetKeyDownImpl(a_Keycode); }
        static bool GetMouseButtonDown(int a_Button) { return s_Instance->GetMouseButtonDownImpl(a_Button); }
        static glm::vec2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
        static bool GetMouseX() { return s_Instance->GetMouseXImpl(); }
        static bool GetMouseY() { return s_Instance->GetMouseYImpl(); }

    protected:
        virtual bool GetKeyDownImpl(int a_Keycode) const = 0;
        virtual bool GetMouseButtonDownImpl(int a_Button) const = 0;
        virtual glm::vec2 GetMousePositionImpl() const = 0;
        virtual float GetMouseXImpl() const = 0;
        virtual float GetMouseYImpl() const = 0;

    private:
        static Input* s_Instance;

    };
}
