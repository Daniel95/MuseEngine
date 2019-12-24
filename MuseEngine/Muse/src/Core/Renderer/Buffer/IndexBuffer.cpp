#include "MusePCH.h"
#include "IndexBuffer.h"
#include "Core/Utilities/Defines.h"
#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/Buffer/OpenGLIndexBuffer.h"

/*
#if MUSE_PLATFORM_WINDOWS
#include "Platform/Direct3D/Direct3DIndexBuffer.h
#endif
 */

namespace Muse
{
    IndexBuffer::~IndexBuffer()
    {
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* a_Indices, uint32_t a_Count)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::OpenGL:
            {
                return new OpenGLIndexBuffer(a_Indices, a_Count);
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
