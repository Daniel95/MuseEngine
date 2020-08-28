#include "MusePCH.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Buffer/BufferLayout.h"
#include "Texture.h"
#include "SubTexture2D.h"

#include "Core/Resource/ResourceManager.h"
#include "RenderCommand.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Muse
{
    Renderer2D::Data Renderer2D::s_Data;

    void Renderer2D::Init()
    {
        MUSE_PROFILE_FUNCTION();

        s_Data.QuadVertexArray = Muse::VertexArray::Create();

        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float, "a_TexIndex" },
            { ShaderDataType::Float, "a_TilingFactor" },
            });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        std::shared_ptr<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;

        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        ResourceManager::Add("WhiteTexture", s_Data.WhiteTexture);

        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
        {
            samplers[i] = i;
        }

        s_Data.ColoredTextureShader = ResourceManager::Load<Shader>("assets/shaders/ColoredTexture.glsl");
        s_Data.ColoredTextureShader->Bind();
        s_Data.ColoredTextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

        // Reset texture slots
        s_Data.TextureSlots[0] = s_Data.WhiteTexture;

        s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        s_Data.TextureCoordinates[0] = { 0.0f, 0.0f };
        s_Data.TextureCoordinates[1] = { 1.0f, 0.0f };
        s_Data.TextureCoordinates[2] = { 1.0f, 1.0f };
        s_Data.TextureCoordinates[3] = { 0.0f, 1.0f };
    }

    void Renderer2D::ShutDown()
    {
        MUSE_PROFILE_FUNCTION();
    }

    void Renderer2D::BeginScene(const glm::mat4& a_ViewProjectionMatrix)
    {
        MUSE_PROFILE_FUNCTION();

        // Update camera in shader
        s_Data.ColoredTextureShader->Bind();
        s_Data.ColoredTextureShader->SetMat4("u_ViewProjection", a_ViewProjectionMatrix);

        Reset();
    }

    void Renderer2D::EndScene()
    {
        MUSE_PROFILE_FUNCTION();

        // Calculate the dataSize, Update the Buffer Data.
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        Flush();
    }

    void Renderer2D::Flush()
    {
        // Bind textures
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        {
            s_Data.TextureSlots[i]->Bind(i);
        }

        // DrawCall
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

        s_Data.Stats.DrawCalls++;
    }

    void Renderer2D::DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, float a_Rotation, const glm::vec4& a_Color)
    {
        MUSE_PROFILE_FUNCTION();

        // White texture, default tiling
        const float textureIndex = 0.0f;
        const float tilingFactor = 0.0f;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), a_Position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(a_Rotation), { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { a_Size.x, a_Size.y, 1.0f });

        DrawQuad(transform, a_Color, textureIndex, tilingFactor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, float a_Rotation, const std::shared_ptr<Texture2D>& a_Texture, float a_TilingFactor, const glm::vec4& a_TintColor)
    {
        MUSE_PROFILE_FUNCTION();

        // Check if the texture is stored
        float textureIndex = 0.0f;
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i].get() == *a_Texture.get())
            {
                textureIndex = (float)i;
            }
        }

        // If the texture is not yet stored assign it to TextureSlots and increase TextureSlotIndex
        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = a_Texture;
            s_Data.TextureSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), a_Position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(a_Rotation), { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { a_Size.x, a_Size.y, 1.0f });

        DrawQuad(transform, a_TintColor, textureIndex, a_TilingFactor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, float a_Rotation, const std::shared_ptr<SubTexture2D>& a_SubTexture, float a_TilingFactor, const glm::vec4& a_TintColor)
    {
        MUSE_PROFILE_FUNCTION();

        const std::shared_ptr<Texture2D> texture = a_SubTexture->GetTexture();

        // Check if the texture is stored
        float textureIndex = 0.0f;
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
            }
        }

        // If the texture is not yet stored assign it to TextureSlots and increase TextureSlotIndex
        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = a_SubTexture->GetTexture();
            s_Data.TextureSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), a_Position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(a_Rotation), { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { a_Size.x, a_Size.y, 1.0f });

        DrawQuad(transform, a_TintColor, textureIndex, a_TilingFactor, a_SubTexture->GetTextureCoords());
    }

    void Renderer2D::DrawQuad(const glm::mat4& a_Transform, const glm::vec4& a_Color)
    {
        MUSE_PROFILE_FUNCTION();

        // White texture, default tiling
        const float textureIndex = 0.0f;
        const float tilingFactor = 0.0f;

        DrawQuad(a_Transform, a_Color, 0, 0);
    }

    void Renderer2D::DrawQuad(const glm::mat4& a_Transform, const std::shared_ptr<Texture2D>& a_Texture, float a_TilingFactor, const glm::vec4& a_TintColor)
    {
        MUSE_PROFILE_FUNCTION();

        // Check if the texture is stored
        float textureIndex = 0.0f;
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i].get() == *a_Texture.get())
            {
                textureIndex = (float)i;
            }
        }

        // If the texture is not yet stored assign it to TextureSlots and increase TextureSlotIndex
        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = a_Texture;
            s_Data.TextureSlotIndex++;
        }

        DrawQuad(a_Transform, a_TintColor, textureIndex, a_TilingFactor);
    }

    void Renderer2D::DrawQuad(const glm::mat4& a_Transform, const std::shared_ptr<SubTexture2D>& a_SubTexture, float a_TilingFactor, const glm::vec4& a_TintColor)
    {
        MUSE_PROFILE_FUNCTION();

        const std::shared_ptr<Texture2D> texture = a_SubTexture->GetTexture();

        // Check if the texture is stored
        float textureIndex = 0.0f;
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
            }
        }

        // If the texture is not yet stored assign it to TextureSlots and increase TextureSlotIndex
        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = a_SubTexture->GetTexture();
            s_Data.TextureSlotIndex++;
        }

        DrawQuad(a_Transform, a_TintColor, textureIndex, a_TilingFactor, a_SubTexture->GetTextureCoords());
    }

    void Renderer2D::DrawQuad(const glm::mat4& a_Transform, const glm::vec4& a_TintColor, int a_TextureIndex, float a_TilingFactor)
    {
        ASSERT_ENGINE(s_Data.QuadVertexBufferPtr != nullptr, "QuadVertexBufferPtr is null! Can only draw after BeginScene and before DrawScene.");

        float x = 7, y = 6;
        float sheetWidth = 2560.0f, sheetHeight = 1664.0f;
        float spriteWidth = 128.0f, spriteHeight = 128.0f;

        glm::vec2 textureCoords[] =
        {
            { (x * spriteWidth) / sheetWidth, (y * spriteHeight) / sheetHeight },
            { ((x + 1) * spriteWidth) / sheetWidth, (y * spriteHeight) / sheetHeight },
            { ((x + 1) * spriteWidth) / sheetWidth, ((y + 1) * spriteHeight) / sheetHeight },
            { (x * spriteWidth) / sheetWidth, ((y + 1) * spriteHeight) / sheetHeight },
        };

        if (s_Data.QuadIndexCount >= Renderer2D::Data::MaxIndices)
        {
            EndScene();
            Reset();
        }

        for (size_t i = 0; i < 4; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = a_Transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = a_TintColor;
            s_Data.QuadVertexBufferPtr->TexCoord = s_Data.TextureCoordinates[i];
            s_Data.QuadVertexBufferPtr->TexIndex = a_TextureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = a_TilingFactor;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::DrawQuad(const glm::mat4& a_Transform, const glm::vec4& a_TintColor, int a_TextureIndex, float a_TilingFactor, const glm::vec2* a_TextureCoords)
    {
        ASSERT_ENGINE(s_Data.QuadVertexBufferPtr != nullptr, "QuadVertexBufferPtr is null! Can only draw after BeginScene and before DrawScene.");

        if (s_Data.QuadIndexCount >= Renderer2D::Data::MaxIndices)
        {
            EndScene();
            Reset();
        }

        for (size_t i = 0; i < 4; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = a_Transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = a_TintColor;
            s_Data.QuadVertexBufferPtr->TexCoord = a_TextureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = a_TextureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = a_TilingFactor;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::Reset()
    {
        // Reset QuadIndexCount and QuadVertexBufferPtr
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        s_Data.TextureSlotIndex = 1;
    }
}
