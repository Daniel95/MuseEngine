#pragma once

#include <string>

namespace Muse
{
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Shader* Create(const std::string& a_FilePath);
        static Shader* Create(const std::string& a_Name, const std::string& a_VertexSrc, const std::string& a_FragmentSrc);
    };
}
