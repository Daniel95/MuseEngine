#pragma once

#include <glm/glm.hpp>

namespace Muse
{
    class Input
    {
    public:
        static bool GetKeyDown(int a_Keycode);
        static bool GetMouseButtonDown(int a_Button);
        static glm::vec2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
        static float GetMouseScrollDelta() { return s_MouseScrollDelta; };

        void UpdateMouseScrollDelta(float a_MouseScrollDelta) { s_MouseScrollDelta = a_MouseScrollDelta; };

    private:
        static float s_MouseScrollDelta;

    };
}
