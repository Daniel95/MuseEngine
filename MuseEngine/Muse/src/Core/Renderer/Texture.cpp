#include "MusePCH.h"
#include "Texture.h"
#include "Renderer.h"
#include "Core/Utilities/Defines.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Muse
{
    std::shared_ptr<Texture> Texture::Load(const std::string& a_Path)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::OpenGL:
            {
                return std::make_shared<OpenGLTexture>(a_Path);
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

    std::shared_ptr<Texture> Texture::Create(uint32_t a_Width, uint32_t a_Height)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::OpenGL:
            {
                return std::make_shared<OpenGLTexture>(a_Width, a_Height);
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
