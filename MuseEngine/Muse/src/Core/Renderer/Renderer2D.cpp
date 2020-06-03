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

        s_Data.TextureCoordinates[0] = { 0.0f, 0.0f };
        s_Data.TextureCoordinates[1] = { 1.0f, 0.0f };
        s_Data.TextureCoordinates[2] = { 1.0f, 1.0f };
        s_Data.TextureCoordinates[3] = { 0.0f, 1.0f };
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

    void Renderer2D::DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, float a_Rotation, const std::shared_ptr<Texture>& a_Texture, float a_TilingFactor, const glm::vec4& a_TintColor)
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

    void Renderer2D::DrawQuad(const glm::mat4& a_Transform, const glm::vec4& a_Color)
    {
        MUSE_PROFILE_FUNCTION();

        // White texture, default tiling
        const float textureIndex = 0.0f;
        const float tilingFactor = 0.0f;

        DrawQuad(a_Transform, a_Color, textureIndex, tilingFactor);
    }

    void Renderer2D::DrawQuad(const glm::mat4& a_Transform, const std::shared_ptr<Texture>& a_Texture, float a_TilingFactor, const glm::vec4& a_TintColor)
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

    void Renderer2D::DrawQuad(const glm::mat4& a_Transform, const glm::vec4& a_TintColor, int a_TextureIndex, float a_TilingFactor)
    {
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
    }
}
