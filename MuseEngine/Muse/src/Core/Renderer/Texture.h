#pragma once

#include <string>
#include <memory>
#include "Core/System/Resource/Resource.h"

namespace Muse
{
    class Texture : public Resource
    {
    public:
        virtual ~Texture() = default;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void Bind(uint32_t a_Slot = 0) const = 0;

        static std::shared_ptr<Texture> Create(const std::string& a_Path);

    };

    class Texture2D : public Texture
    {

    };
}
