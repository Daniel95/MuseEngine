#include "MusePCH.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"
#include "Core/Utilities/Defines.h"
#include "Core/Utilities/Log.h"

namespace Muse
{
    OpenGLContext::OpenGLContext(GLFWwindow* a_WindowHandle)
        : m_WindowHandle(a_WindowHandle)
    {
        ASSERT_ENGINE(a_WindowHandle, "Handle is null!");
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        const int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        ASSERT_ENGINE(status, "Failed to initialize Glad!");

        LOG_ENGINE_INFO("OpenGL Info:");
        LOG_ENGINE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        LOG_ENGINE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        LOG_ENGINE_INFO("  Version: {0}", glGetString(GL_VERSION));
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}
