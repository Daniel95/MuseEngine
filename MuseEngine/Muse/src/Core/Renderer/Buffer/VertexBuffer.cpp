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
    VertexBuffer* VertexBuffer::Create(float* a_Vertices, uint32_t a_Size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::OpenGL:
            {
                return new OpenGLVertexBuffer(a_Vertices, a_Size);
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
