#include "MusePCH.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"
#include "Core/Renderer/Buffer/BufferLayout.h"

namespace Muse
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType a_ShaderDataType)
    {
        MUSE_PROFILE_FUNCTION();

        switch (a_ShaderDataType)
        {
            case ShaderDataType::Float:    return GL_FLOAT;
            case ShaderDataType::Float2:   return GL_FLOAT;
            case ShaderDataType::Float3:   return GL_FLOAT;
            case ShaderDataType::Float4:   return GL_FLOAT;
            case ShaderDataType::Mat3:     return GL_FLOAT;
            case ShaderDataType::Mat4:     return GL_FLOAT;
            case ShaderDataType::Int:      return GL_INT;
            case ShaderDataType::Int2:     return GL_INT;
            case ShaderDataType::Int3:     return GL_INT;
            case ShaderDataType::Int4:     return GL_INT;
            case ShaderDataType::Bool:     return GL_BOOL;
        }

        ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        MUSE_PROFILE_FUNCTION();

        glCreateVertexArrays(1, &m_RendererId);
    }

    void OpenGLVertexArray::Bind() const
    {
        MUSE_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererId);
    }

    void OpenGLVertexArray::Unbind() const
    {
        MUSE_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& a_VertexBuffer)
    {
        MUSE_PROFILE_FUNCTION();

        ASSERT_ENGINE(!a_VertexBuffer->GetLayout().GetElements().empty(), "Vertex buffer has no layout!");

        glBindVertexArray(m_RendererId);

        a_VertexBuffer->Bind();

        const auto& layout = a_VertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            switch (element.Type)
            {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool:
                {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribPointer(m_VertexBufferIndex,
                        element.GetComponentCount(),
                        ShaderDataTypeToOpenGLBaseType(element.Type),
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        layout.GetStride(),
                        (const void*)element.Offset);
                    m_VertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                {
                    uint8_t count = element.GetComponentCount();
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(m_VertexBufferIndex);
                        glVertexAttribPointer(m_VertexBufferIndex,
                            count,
                            ShaderDataTypeToOpenGLBaseType(element.Type),
                            element.Normalized ? GL_TRUE : GL_FALSE,
                            layout.GetStride(),
                            (const void*)(element.Offset + sizeof(float) * count * i));
                        glVertexAttribDivisor(m_VertexBufferIndex, 1);
                        m_VertexBufferIndex++;
                    }
                    break;
                }
                default:
                    ASSERT_ENGINE(false, "Unknown ShaderDataType!");
            }
        }

        m_VertexBuffers.push_back(a_VertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& a_IndexBuffer)
    {
        MUSE_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererId);
        a_IndexBuffer->Bind();

        m_IndexBuffer = a_IndexBuffer;
    }
}
