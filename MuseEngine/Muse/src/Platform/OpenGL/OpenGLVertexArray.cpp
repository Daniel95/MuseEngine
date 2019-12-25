#include "MusePCH.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"
#include "Core/Renderer/Buffer/BufferLayout.h"

namespace Muse
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType a_ShaderDataType)
    {
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
        glCreateVertexArrays(1, &m_RendererId);
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererId);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& a_VertexBuffer)
    {
        ASSERT(!a_VertexBuffer->GetLayout().GetElements().empty(), "Vertex buffer has no layout!");

        glBindVertexArray(m_RendererId);

        a_VertexBuffer->Bind();

        uint32_t index = 0;
        const auto& layout = a_VertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            //Location, Amount of type, Type, normalized, stride (vertex size), pointer
            glVertexAttribPointer(index,
                element.GetNumberCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)element.Offset);
            index++;
        }

        m_VertexBuffers.push_back(a_VertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& a_IndexBuffer)
    {
        glBindVertexArray(m_RendererId);
        a_IndexBuffer->Bind();
        m_IndexBuffer = a_IndexBuffer;
    }
}
