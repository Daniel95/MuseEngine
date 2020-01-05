#include "MusePCH.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Core/Utilities/Defines.h"
#include "Renderer.h"
#include "Core/Instrumentor.h"

namespace Muse
{
    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        MUSE_PROFILE_FUNCTION();

        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::OpenGL:
            {
                return std::make_shared<OpenGLVertexArray>();
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
