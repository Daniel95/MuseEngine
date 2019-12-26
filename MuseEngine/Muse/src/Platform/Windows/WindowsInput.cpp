#include "MusePCH.h"
#include "WindowsInput.h"
#include "Core/Application.h"
#include "GLFW/glfw3.h"
#include "Core/Window.h"

namespace Muse
{
    Input* Input::s_Instance = new WindowsInput();

    bool WindowsInput::IsKeyPressedImpl(int a_Keycode) const
    {
        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        const auto state = glfwGetKey(window, a_Keycode);
        return state == static_cast<const int>(GLFW_PRESS || GLFW_REPEAT);
    }

    bool WindowsInput::IsMouseButtonPressedImpl(int a_Button) const
    {
        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        const auto state = glfwGetMouseButton(window, a_Button);
        return state == GLFW_PRESS;
    }

    glm::vec2 WindowsInput::GetMousePositionImpl() const
    {
        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return glm::vec2(xPos, yPos);
    }

    float WindowsInput::GetMouseXImpl() const
    {
        glm::vec2 mousePosition = GetMousePositionImpl();
        return mousePosition.x;
    }

    float WindowsInput::GetMouseYImpl() const
    {
        glm::vec2 mousePosition = GetMousePositionImpl();
        return mousePosition.y;
    }
}
