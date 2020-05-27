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
        OpenGLShader(const std::string& a_VertexSource, const std::string& a_FragmentSource);
        OpenGLShader(const std::string& a_FilePath);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetInt(const std::string& a_Name, int a_Value) override;
        virtual void SetIntArray(const std::string& a_Name, int* a_Values, uint32_t count) override;
        virtual void SetFloat(const std::string& a_Name, float a_Value) override;
        virtual void SetFloat3(const std::string& a_Name, const glm::vec3& a_Value) override;
        virtual void SetFloat4(const std::string& a_Name, const glm::vec4& a_Value) override;
        virtual void SetMat4(const std::string& a_Name, const glm::mat4& a_Value) override;

        void Compile(const std::string& a_VertexSource, const std::string& a_FragmentSource) override;

        void UploadUniformInt(const std::string& a_Name, int a_Value);
        void UploadUniformIntArray(const std::string& a_Name, int* values, uint32_t a_Count);

        void UploadUniformFloat(const std::string& name, float a_Value);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& a_Value);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& a_Value);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& a_Value);

        void UploadUniformMat3(const std::string& a_Name, const glm::mat3& a_Matrix);
        void UploadUniformMat4(const std::string& a_Name, const glm::mat4& a_Matrix);

    private:
        uint32_t m_RendererID;

        static std::string ReadFile(const std::string& a_Filepath);
        static std::unordered_map<GLenum, std::string> Preprocess(const std::string& a_Source);
        void Compile(const std::unordered_map<GLenum, std::string>& a_ShaderSources);

    };
}
