#pragma once

#include <string>
#include <vector>
#include "Core/Utilities/Defines.h"

namespace Muse
{
    enum class ShaderDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool,
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType a_ShaderDataType)
    {
        switch (a_ShaderDataType)
        {
            case ShaderDataType::Float:    return 4;
            case ShaderDataType::Float2:   return 4 * 2;
            case ShaderDataType::Float3:   return 4 * 3;
            case ShaderDataType::Float4:   return 4 * 4;
            case ShaderDataType::Mat3:     return 4 * 3 * 3;
            case ShaderDataType::Mat4:     return 4 * 4 * 4;
            case ShaderDataType::Int:      return 4;
            case ShaderDataType::Int2:     return 4 * 2;
            case ShaderDataType::Int3:     return 4 * 3;
            case ShaderDataType::Int4:     return 4 * 4;
            case ShaderDataType::Bool:     return 1;
        }

        ASSERT_ENGINE(false, "Unknown ShaderDataType!");
        return 0;
    }


    struct BufferElement
    {
        ShaderDataType Type;
        std::string Name;
        uint32_t Size;
        uint32_t Offset;
        bool Normalized;

        BufferElement() = default;

        BufferElement(ShaderDataType a_ShaderDataType, const std::string& a_Name, bool a_Normalized = false)
            : Type(a_ShaderDataType), Name(a_Name), Size(ShaderDataTypeSize(a_ShaderDataType)), Offset(0), Normalized(a_Normalized)
        {
        }

        uint32_t GetNumberCount() const
        {
            switch (Type)
            {
                case ShaderDataType::Float:   return 1;
                case ShaderDataType::Float2:  return 2;
                case ShaderDataType::Float3:  return 3;
                case ShaderDataType::Float4:  return 4;
                case ShaderDataType::Mat3:    return 3 * 3;
                case ShaderDataType::Mat4:    return 4 * 4;
                case ShaderDataType::Int:     return 1;
                case ShaderDataType::Int2:    return 2;
                case ShaderDataType::Int3:    return 3;
                case ShaderDataType::Int4:    return 4;
                case ShaderDataType::Bool:    return 1;
            }

            ASSERT(false, "Unknown ShaderDataType!");
            return 0;
        }
    };

     class BufferLayout
     {
     public:
         BufferLayout() = default;

         BufferLayout(const std::initializer_list<BufferElement>& a_Elements)
             : m_Elements(a_Elements)
         {
             CalculateOffsetAndStride();
         }

         uint32_t GetStride() const { return m_Stride; }
         const std::vector<BufferElement>& GetElements() const { return m_Elements; }

         std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
         std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
         std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
         std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
     private:
         std::vector<BufferElement> m_Elements;
         uint32_t m_Stride = 0;

         void CalculateOffsetAndStride();
         
     };
}

