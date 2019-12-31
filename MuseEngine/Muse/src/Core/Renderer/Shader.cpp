#include "MusePCH.h"
#include "Shader.h"
#include "Renderer.h"
#include "Core/Utilities/Defines.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Muse
{
    void Shader::Bind() const
    {
    }

    void Shader::Unbind() const
    {
    }

    std::shared_ptr<Shader> Shader::Create(const std::string& a_FilePath)
    {
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

    std::shared_ptr<Shader> Shader::Create(const std::string& a_Name, const std::string& a_VertexSrc, const std::string& a_FragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::OpenGL:
            {
                return std::make_shared<OpenGLShader>(a_Name, a_VertexSrc, a_FragmentSrc);
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
