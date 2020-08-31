#include "MusePCH.h"
#include "Core/Input/Input.h"
#include "Core/Application.h"
#include "Core/Window.h"
#include "GLFW/glfw3.h"

#include "Core/Input/KeyCodes.h"
#include "Core/Input/MouseButtonCodes.h"

namespace Muse
{
    bool Input::GetKeyDown(KeyCode a_Keycode)
    {
        MUSE_PROFILE_FUNCTION();

        if (s_BlockInput) { return false; }

        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        const auto state = glfwGetKey(window, static_cast<int32_t>(a_Keycode));
        return state == static_cast<const int>(GLFW_PRESS || GLFW_REPEAT);
    }

    bool Input::GetMouseButtonDown(MouseCode a_Button)
    {
        MUSE_PROFILE_FUNCTION();

        if (s_BlockInput) { return false; }

        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        const auto state = glfwGetMouseButton(window, static_cast<int32_t>(a_Button));
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::GetMousePosition()
    {
        MUSE_PROFILE_FUNCTION();

        if (s_BlockInput) { return {0, 0}; }

        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return glm::vec2(xPos, yPos);
    }

    float Input::GetMouseX()
    {
        MUSE_PROFILE_FUNCTION();

        glm::vec2 mousePosition = GetMousePosition();
        return mousePosition.x;
    }

    float Input::GetMouseY()
    {
        MUSE_PROFILE_FUNCTION();

        glm::vec2 mousePosition = GetMousePosition();
        return mousePosition.y;
    }
}
