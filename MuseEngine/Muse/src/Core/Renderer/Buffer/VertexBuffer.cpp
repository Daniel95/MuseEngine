#include "MusePCH.h"
#include "VertexBuffer.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Utilities/Defines.h"
#include "Platform/OpenGL/Buffer/OpenGLVertexBuffer.h"

/*
#if MUSE_PLATFORM_WINDOWS
#include "Platform/Direct3D/Direct3DVertexBuffer.h
#endif
 */

namespace Muse
{
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t a_Size)
    {
        MUSE_PROFILE_FUNCTION();

        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::OpenGL:
            {
                return std::make_shared<OpenGLVertexBuffer>(a_Size);
            }
            default:
            {
                std::string message = "RenderAPI: " + static_cast<int>(Renderer::GetAPI());
                message.append(" is not supported!");
                ASSERT(false, message);
                return nullptr;
            }
        }

        return std::shared_ptr<VertexBuffer>();
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* a_Vertices, uint32_t a_Size)
    {
        MUSE_PROFILE_FUNCTION();

        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::OpenGL:
            {
                return std::make_shared<OpenGLVertexBuffer>(a_Vertices, a_Size);
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
