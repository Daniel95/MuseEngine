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
    struct Renderer2DStorage
    {
        std::shared_ptr<VertexArray> QuadVertexArray;
        std::shared_ptr<Shader> ColoredTextureShader;
        std::shared_ptr<Texture> WhiteTexture;
    };

    static Renderer2DStorage* s_Data;
    
    void Renderer2D::Init()
    {
        MUSE_PROFILE_FUNCTION();

        s_Data = new Renderer2DStorage();
        s_Data->QuadVertexArray = Muse::VertexArray::Create();

        float quadVertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        std::shared_ptr<VertexBuffer> squareVB = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
        squareVB->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
        });
        s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<IndexBuffer> quadIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        s_Data->QuadVertexArray->SetIndexBuffer(quadIB);

        s_Data->WhiteTexture = ResourceManager::Create<Texture>("WhiteTexture", 1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        s_Data->ColoredTextureShader = ResourceManager::Load<Shader>("assets/shaders/ColoredTexture.glsl");
        s_Data->ColoredTextureShader->Bind();
        s_Data->ColoredTextureShader->SetInt("u_Texture", 0);
    }

    void Renderer2D::ShutDown()
    {
        MUSE_PROFILE_FUNCTION();

        delete s_Data;
    }

    void Renderer2D::BeginScene(const CameraComponent& a_OrthographicCamera)
    {
        MUSE_PROFILE_FUNCTION();

        s_Data->ColoredTextureShader->Bind();
        s_Data->ColoredTextureShader->SetMat4("u_ViewProjection", a_OrthographicCamera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene()
    {
        MUSE_PROFILE_FUNCTION();
    }

    void Renderer2D::DrawQuad(const QuadPropertiesTransform& a_QuadPropertiesTransform)
    {
        MUSE_PROFILE_FUNCTION();

        if(a_QuadPropertiesTransform.Texture != nullptr)
        {
            a_QuadPropertiesTransform.Texture->Bind();
        }
        else
        {
            s_Data->WhiteTexture->Bind();
        }

        s_Data->ColoredTextureShader->Bind();
        s_Data->ColoredTextureShader->SetFloat4("u_Color", a_QuadPropertiesTransform.Color);
        s_Data->ColoredTextureShader->SetMat4("u_Transform", a_QuadPropertiesTransform.Transform);
        s_Data->ColoredTextureShader->SetFloat1("u_TilingFactor", 1);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
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
            s_Data->WhiteTexture->Bind();
        }

        s_Data->ColoredTextureShader->Bind();
        s_Data->ColoredTextureShader->SetFloat4("u_Color", a_QuadProperties.Color);
        s_Data->ColoredTextureShader->SetMat4("u_Transform", transform);
        s_Data->ColoredTextureShader->SetFloat1("u_TilingFactor", 1);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

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

        s_Data->ColoredTextureShader->Bind();
        s_Data->ColoredTextureShader->SetFloat4("u_Color", glm::vec4(1));
        s_Data->ColoredTextureShader->SetMat4("u_Transform", a_Transform);
        s_Data->ColoredTextureShader->SetFloat1("u_TilingFactor", 1);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
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

        s_Data->WhiteTexture->Bind();

        s_Data->ColoredTextureShader->Bind();
        s_Data->ColoredTextureShader->SetFloat4("u_Color", a_Color);
        s_Data->ColoredTextureShader->SetMat4("u_Transform", a_Transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
    */
}
