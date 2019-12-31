#pragma once
#include "Core/Renderer/Shader.h"

// TODO: REMOVE!
typedef unsigned int GLenum;

namespace Muse 
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& a_FilePath);
        OpenGLShader(const std::string& a_Name, const std::string& a_VertexSrc, const std::string& a_FragmentSrc);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        const std::string& GetName() override { return m_Name; }

        void UploadUniformInt(const std::string& a_Name, int a_Int) const;
        void UploadUniformFloat(const std::string& a_Name, float a_Float) const;
        void UploadUniformFloat2(const std::string& a_Name, const glm::vec2& a_Float2) const;
        void UploadUniformFloat3(const std::string& a_Name, const glm::vec3& a_Float3) const;
        void UploadUniformFloat4(const std::string& a_Name, const glm::vec4& a_Float4) const;
        void UploadUniformMat3(const std::string& a_Name, const glm::mat3& a_Matrix) const;
        void UploadUniformMat4(const std::string& a_Name, const glm::mat4& a_Matrix) const;

    private:
        uint32_t m_RendererID;
        std::string m_Name;

        std::string ReadFile(const std::string& a_Filepath) const;
        std::unordered_map<GLenum, std::string> Preprocess(const std::string& a_Source) const;
        void Compile(const std::unordered_map<GLenum, std::string>& a_ShaderSources);
    };
}
