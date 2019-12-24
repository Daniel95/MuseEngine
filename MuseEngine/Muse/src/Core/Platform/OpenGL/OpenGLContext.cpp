#include "MusePCH.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"
#include "Core/Utilities/Defines.h"

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
    }

    void OpenGLContext::SwapBuffers()
    {
        //glBegin(GL_TRIANGLES);

        glfwSwapBuffers(m_WindowHandle);
    }
}
