#include "MusePCH.h"
#include "Renderer2D.h"

#include "Core/Gameplay/Component/CameraComponent.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Buffer/BufferLayout.h"
#include "Texture.h"

#include "Core/Resource/ResourceManager.h"
#include "RenderCommand.h"

namespace Muse
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;
    };

    struct Renderer2DStorage
    {
        static const uint32_t MaxQuads = 20000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

        std::shared_ptr<VertexArray> QuadVertexArray;
        std::shared_ptr<VertexBuffer> QuadVertexBuffer;
        std::shared_ptr<Shader> ColoredTextureShader;
        std::shared_ptr<Texture> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferPtr = nullptr;
        QuadVertex* QuadVertexBufferBase = nullptr;

        std::array<std::shared_ptr<Texture>, MaxTextureSlots> TextureSlots;
        uint16_t TextureSlotIndex = 1; // 0 = white texture

        glm::vec4 QuadVertexPositions[4];
    };

    static Renderer2DStorage s_Data;

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

        s_Data.WhiteTexture = Texture::Create(1, 1);
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
    }

    void Renderer2D::ShutDown()
    {
        MUSE_PROFILE_FUNCTION();
    }

    void Renderer2D::BeginScene(const CameraComponent& a_OrthographicCamera)
    {
        MUSE_PROFILE_FUNCTION();

        // Update camera in shader
        s_Data.ColoredTextureShader->Bind();
        s_Data.ColoredTextureShader->SetMat4("u_ViewProjection", a_OrthographicCamera.GetViewProjectionMatrix());
         
        // Reset QuadIndexCount and QuadVertexBufferPtr
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::EndScene()
    {
        MUSE_PROFILE_FUNCTION();

        // Calculate the dataSize, Update the Buffer Data.
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        // Bind textures
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        {
            s_Data.TextureSlots[i]->Bind(i);
        }

        // DrawCall
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
    }

    void Renderer2D::DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, const glm::vec4& a_Color)
    {
        MUSE_PROFILE_FUNCTION();

        // White texture, default tiling
        const float texIndex = 0.0f;
        const float tilingFactor = 0.0f;

        s_Data.QuadVertexBufferPtr->Position = a_Position;
        s_Data.QuadVertexBufferPtr->Color = a_Color;
        s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 0.0f);
        s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { a_Position.x + a_Size.x, a_Position.y, 0.0f };
        s_Data.QuadVertexBufferPtr->Color = a_Color;
        s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 0.0f);
        s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { a_Position.x + a_Size.x, a_Position.y + a_Size.y, 0.0f };;
        s_Data.QuadVertexBufferPtr->Color = a_Color;
        s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 1.0f);
        s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { a_Position.x, a_Position.y + a_Size.y, 0.0f };;
        s_Data.QuadVertexBufferPtr->Color = a_Color;
        s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 1.0f);
        s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadIndexCount += 6;
    }

    void Renderer2D::DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, const std::shared_ptr<Texture>& a_Texture, float a_TilingFactor, const glm::vec4& a_TintColor)
    {
        MUSE_PROFILE_FUNCTION();

        const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

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

        s_Data.QuadVertexBufferPtr->Position = a_Position;
        s_Data.QuadVertexBufferPtr->Color = a_TintColor;
        s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 0.0f);
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = a_TilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { a_Position.x + a_Size.x, a_Position.y, 0.0f };
        s_Data.QuadVertexBufferPtr->Color = a_TintColor;
        s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 0.0f);
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = a_TilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { a_Position.x + a_Size.x, a_Position.y + a_Size.y, 0.0f };;
        s_Data.QuadVertexBufferPtr->Color = a_TintColor;
        s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 1.0f);
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = a_TilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { a_Position.x, a_Position.y + a_Size.y, 0.0f };;
        s_Data.QuadVertexBufferPtr->Color = a_TintColor;
        s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 1.0f);
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = a_TilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadIndexCount += 6;
    }

    void Renderer2D::DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, float a_Rotation, const std::shared_ptr<Texture>& a_Texture, float a_TilingFactor, const glm::vec4& a_TintColor)
    {
        MUSE_PROFILE_FUNCTION();

        const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

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

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBufferPtr->Color = a_TintColor;
        s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 0.0f);
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = a_TilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBufferPtr->Color = a_TintColor;
        s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 0.0f);
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = a_TilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = a_TintColor;
        s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 1.0f);
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = a_TilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBufferPtr->Color = a_TintColor;
        s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 1.0f);
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = a_TilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadIndexCount += 6;
    }

    /*
    void Renderer2D::DrawQuad(const QuadPropertiesTransform& a_QuadPropertiesTransform)
    {
        MUSE_PROFILE_FUNCTION();

        if (a_QuadPropertiesTransform.Texture != nullptr)
        {
            a_QuadPropertiesTransform.Texture->Bind();
        }
        else
        {
            s_Data.WhiteTexture->Bind();
        }

        s_Data.ColoredTextureShader->Bind();
        s_Data.ColoredTextureShader->SetFloat4("u_Color", a_QuadPropertiesTransform.Color);
        s_Data.ColoredTextureShader->SetMat4("u_Transform", a_QuadPropertiesTransform.Transform);
        s_Data.ColoredTextureShader->SetFloat1("u_TilingFactor", 1);

        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const QuadProperties& a_QuadProperties)
    {
        MUSE_PROFILE_FUNCTION();

        glm::mat4 transform;

        if(a_QuadProperties.Rotation != 0)
        {
            transform = glm::translate(glm::mat4(1.0f), a_QuadProperties.Position)
                * glm::rotate(glm::mat4(1.0f), a_QuadProperties.Rotation, { 0.0f, 0.0f, 1.0f })
                * glm::scale(glm::mat4(1.0f), { a_QuadProperties.Size.x, a_QuadProperties.Size.y, 1.0f });
        }
        else
        {
            transform = glm::translate(glm::mat4(1.0f), a_QuadProperties.Position)
                * glm::scale(glm::mat4(1.0f), { a_QuadProperties.Size.x, a_QuadProperties.Size.y, 1.0f });
        }

        if (a_QuadProperties.Texture != nullptr)
        {
            a_QuadProperties.Texture->Bind();
        }
        else
        {
            s_Data.WhiteTexture->Bind();
        }

        s_Data.ColoredTextureShader->Bind();
        s_Data.ColoredTextureShader->SetFloat4("u_Color", a_QuadProperties.Color);
        s_Data.ColoredTextureShader->SetMat4("u_Transform", transform);
        s_Data.ColoredTextureShader->SetFloat1("u_TilingFactor", 1);

        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }
    */

    /*
    void Renderer2D::DrawQuad(const glm::vec2& a_Position, const glm::vec2& a_Size, std::shared_ptr<Texture> a_Texture)
    {
        MUSE_PROFILE_FUNCTION();

        DrawQuad({ a_Position.x, a_Position.y, 0.0f }, a_Size, a_Texture);
    }

    void Renderer2D::DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, std::shared_ptr<Texture> a_Texture)
    {
        MUSE_PROFILE_FUNCTION();

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), a_Position) * glm::scale(glm::mat4(1.0f), { a_Size.x, a_Size.y, 1.0f });
        DrawQuad(transform, a_Texture);
    }

    void Renderer2D::DrawQuad(const glm::mat4& a_Transform, std::shared_ptr<Texture> a_Texture)
    {
        MUSE_PROFILE_FUNCTION();

        a_Texture->Bind();

        s_Data.ColoredTextureShader->Bind();
        s_Data.ColoredTextureShader->SetFloat4("u_Color", glm::vec4(1));
        s_Data.ColoredTextureShader->SetMat4("u_Transform", a_Transform);
        s_Data.ColoredTextureShader->SetFloat1("u_TilingFactor", 1);

        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& a_Position, const glm::vec2& a_Size, const glm::vec4& a_Color)
    {
        MUSE_PROFILE_FUNCTION();

        DrawQuad({ a_Position.x, a_Position.y, 0.0f }, a_Size, a_Color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, const glm::vec4& a_Color)
    {
        MUSE_PROFILE_FUNCTION();

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), a_Position) * glm::scale(glm::mat4(1.0f), { a_Size.x, a_Size.y, 1.0f });
        DrawQuad(transform, a_Color);
    }

    void Renderer2D::DrawQuad(const glm::mat4& a_Transform, const glm::vec4& a_Color)
    {
        MUSE_PROFILE_FUNCTION();

        s_Data.WhiteTexture->Bind();

        s_Data.ColoredTextureShader->Bind();
        s_Data.ColoredTextureShader->SetFloat4("u_Color", a_Color);
        s_Data.ColoredTextureShader->SetMat4("u_Transform", a_Transform);

        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }
    */
}
