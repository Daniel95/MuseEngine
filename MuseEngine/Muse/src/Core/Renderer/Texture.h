#pragma once

#include <string>
#include <memory>
#include "Core/Resource/Resource.h"

namespace Muse
{
    class Texture : public Resource
    {
    public:
        virtual ~Texture() = default;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void SetData(void* a_Data, uint32_t a_Size) = 0;
        virtual void SetDataF(void* a_Data, uint32_t a_Size) = 0;

        virtual void Bind(uint32_t a_Slot = 0) const = 0;

        static std::shared_ptr<Texture> Load(const std::string& a_Path);
        static std::shared_ptr<Texture> Create(uint32_t a_Width, uint32_t a_Height);

    };

    class Texture2D : public Texture
    {
    public:
        static std::shared_ptr<Texture2D> Load(const std::string& a_Path) { return std::static_pointer_cast<Texture2D>(Texture::Load(a_Path)); }
        static std::shared_ptr<Texture2D> Create(uint32_t a_Width, uint32_t a_Height) { return std::static_pointer_cast<Texture2D>(Texture::Create(a_Width, a_Height)); }
    };
}
