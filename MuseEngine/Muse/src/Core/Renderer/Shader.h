#pragma once

#include <string>
#include "Core/System/Resource/Resource.h"

namespace Muse
{
    class Shader : public Resource
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void Compile(const std::string& a_VertexSource, const std::string& a_FragmentSource) = 0;

        static std::shared_ptr<Shader> Create(const std::string& a_FilePath);

        bool GetIsCompiled() const { return m_IsCompiled; }

    protected:
        bool m_IsCompiled = false;

    };
}
