#include "MusePCH.h"
#include "WindowsInput.h"
#include "Core/Application.h"
#include "GLFW/glfw3.h"
#include "Core/Window.h"

namespace Muse
{
    Input* Input::s_Instance = new WindowsInput();

    bool WindowsInput::GetKeyDownImpl(int a_Keycode) const
    {
        MUSE_PROFILE_FUNCTION();

        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        const auto state = glfwGetKey(window, a_Keycode);
        return state == static_cast<const int>(GLFW_PRESS || GLFW_REPEAT);
    }

    bool WindowsInput::GetMouseButtonDownImpl(int a_Button) const
    {
        MUSE_PROFILE_FUNCTION();

        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        const auto state = glfwGetMouseButton(window, a_Button);
        return state == GLFW_PRESS;
    }

    glm::vec2 WindowsInput::GetMousePositionImpl() const
    {
        MUSE_PROFILE_FUNCTION();

        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return glm::vec2(xPos, yPos);
    }

    float WindowsInput::GetMouseXImpl() const
    {
        MUSE_PROFILE_FUNCTION();

        glm::vec2 mousePosition = GetMousePositionImpl();
        return mousePosition.x;
    }

    float WindowsInput::GetMouseYImpl() const
    {
        MUSE_PROFILE_FUNCTION();

        glm::vec2 mousePosition = GetMousePositionImpl();
        return mousePosition.y;
    }
}
