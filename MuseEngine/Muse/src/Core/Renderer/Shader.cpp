#include "MusePCH.h"
#include "Shader.h"
#include "Renderer.h"
#include "Core/Utilities/Defines.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Core/Instrumentor.h"

namespace Muse
{
    std::shared_ptr<Shader> Shader::Load(const std::string& a_FilePath)
    {
        MUSE_PROFILE_FUNCTION();

        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::OpenGL:
            {
                return std::make_shared<OpenGLShader>(a_FilePath);
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

    std::shared_ptr<Shader> Shader::Create(const std::string& a_VertexSource, const std::string& a_FragmentSource)
    {
        MUSE_PROFILE_FUNCTION();

        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::OpenGL:
            {
                return std::make_shared<OpenGLShader>(a_VertexSource, a_FragmentSource);
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
