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

        virtual const std::string& GetName() = 0;

        static std::shared_ptr<Shader> Create(const std::string& a_FilePath);
        static std::shared_ptr<Shader> Create(const std::string& a_Name, const std::string& a_VertexSrc, const std::string& a_FragmentSrc);


    };
}
