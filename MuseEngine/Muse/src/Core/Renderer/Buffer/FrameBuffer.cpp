#include "MusePCH.h"
#include "FrameBuffer.h"
#include "Core/Instrumentor.h"
#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/Buffer/OpenGLFrameBuffer.h"

namespace Muse
{
    std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferProperties& spec)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    ASSERT_ENGINE(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLFrameBuffer>(spec);
        }

        ASSERT_ENGINE(false, "Unknown RendererAPI!");
        return nullptr;
    }

}

/*
namespace Muse
{
    std::shared_ptr<FrameBuffer> FrameBuffer::Create(uint32_t a_Width, uint32_t a_Height, FramebufferFormat a_Format)
    {
        MUSE_PROFILE_FUNCTION();

        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::OpenGL:
            {
                return std::make_shared<OpenGLFrameBuffer>(a_Width, a_Height, a_Format);
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

*/