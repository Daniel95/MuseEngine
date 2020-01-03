#pragma once
#include "Core/Renderer/Shader.h"

#include <string>

// TODO: REMOVE!
typedef unsigned int GLenum;

namespace Muse 
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& a_FilePath);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetInt(const std::string& a_Name, int a_Int) override;
        virtual void SetFloat1(const std::string& a_Name, float a_Float) override;
        virtual void SetFloat2(const std::string& a_Name, const glm::vec2& a_Float2) override;
        virtual void SetFloat3(const std::string& a_Name, const glm::vec3& a_Float3) override;
        virtual void SetFloat4(const std::string& a_Name, const glm::vec4& a_Float4) override;
        virtual void SetMat3(const std::string& a_Name, const glm::mat3& a_Mat3) override;
        virtual void SetMat4(const std::string& a_Name, const glm::mat4& a_Mat4) override;

        void Compile(const std::string& a_VertexSource, const std::string& a_FragmentSource) override;

    private:
        uint32_t m_RendererID;

        static std::string ReadFile(const std::string& a_Filepath);
        static std::unordered_map<GLenum, std::string> Preprocess(const std::string& a_Source);
        void Compile(const std::unordered_map<GLenum, std::string>& a_ShaderSources);

    };
}
