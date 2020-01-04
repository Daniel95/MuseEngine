#pragma once

#include <string>
#include "Core/Resource/Resource.h"

namespace Muse
{
    class Shader : public Resource
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetInt(const std::string& a_Name, int a_Int) = 0;
        virtual void SetFloat1(const std::string& a_Name, float a_Float1) = 0;
        virtual void SetFloat2(const std::string& a_Name, const glm::vec2& a_Float2) = 0;
        virtual void SetFloat3(const std::string& a_Name, const glm::vec3& a_Float3) = 0;
        virtual void SetFloat4(const std::string& a_Name, const glm::vec4& a_Float4) = 0;
        virtual void SetMat3(const std::string& a_Name, const glm::mat3& a_Mat3) = 0;
        virtual void SetMat4(const std::string& a_Name, const glm::mat4& a_Mat4) = 0;

        virtual void Compile(const std::string& a_VertexSource, const std::string& a_FragmentSource) = 0;

        static std::shared_ptr<Shader> Load(const std::string& a_FilePath);
        static std::shared_ptr<Shader> Create(const std::string& a_VertexSource, const std::string& a_FragmentSource);

        bool GetIsCompiled() const { return m_IsCompiled; }

    protected:
        bool m_IsCompiled = false;

    };
}
