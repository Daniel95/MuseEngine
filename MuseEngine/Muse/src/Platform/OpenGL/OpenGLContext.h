#pragma once

#include "Core/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Muse
{
    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* a_WindowHandle);

        virtual void Init() override;
        virtual void SwapBuffers() override;
    private:
        GLFWwindow* m_WindowHandle;

    };
}
