#include "MusePCH.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Core/Utilities/Defines.h"
#include "Renderer.h"

namespace Muse
{
    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::OpenGL:
            {
                return new OpenGLVertexArray();
            }
            default:
            {
                std::string message = "RenderAPI: " + static_cast<int>(Renderer::GetAPI());
                message.append(" is not supported!");
                ASSERT(false, message);
                return nullptr;
            }
        }
    }
}