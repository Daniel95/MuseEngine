#pragma once

#include <glm/glm.hpp>

#include "Core/Input/KeyCodes.h"
#include "Core/Input/MouseButtonCodes.h"

namespace Muse
{
    class Input
    {
    public:
        static bool GetKeyDown(KeyCode a_Keycode);
        static bool GetMouseButtonDown(MouseCode a_Button);
        static glm::vec2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
        static float GetMouseScrollDelta() { return s_MouseScrollDelta; };
        static void SetBlockInput(bool a_Block) { s_BlockInput = a_Block; };

        void UpdateMouseScrollDelta(float a_MouseScrollDelta);

    private:
        static float s_MouseScrollDelta;
        static bool s_BlockInput;

    };
}
