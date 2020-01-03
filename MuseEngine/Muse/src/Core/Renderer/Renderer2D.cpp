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
        std::shared_ptr<Shader> FlatColorShader;
        std::shared_ptr<Shader> TextureShader;
        std::shared_ptr<Texture> WhiteTexture;
    };

    static Renderer2DStorage* s_Data;
    
    void Renderer2D::Init()
    {
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

        s_Data->FlatColorShader = ResourceManager::Load<Shader>("assets/shaders/FlatColor.glsl");
        s_Data->TextureShader = ResourceManager::Load<Shader>("assets/shaders/Texture.glsl");
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetInt("u_Texture", 0);
    }

    void Renderer2D::ShutDown()
    {
        delete s_Data;
    }

    void Renderer2D::BeginScene(const CameraComponent& a_OrthographicCamera)
    {
        s_Data->FlatColorShader->Bind();
        s_Data->FlatColorShader->SetMat4("u_ViewProjection", a_OrthographicCamera.GetViewProjectionMatrix());

        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetMat4("u_ViewProjection", a_OrthographicCamera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene()
    {
    }

    void Renderer2D::DrawQuad(const glm::vec2& a_Position, const glm::vec2& a_Size, std::shared_ptr<Texture> a_Texture)
    {
        DrawQuad({ a_Position.x, a_Position.y, 0.0f }, a_Size, a_Texture);
    }

    void Renderer2D::DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, std::shared_ptr<Texture> a_Texture)
    {
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), a_Position) * glm::scale(glm::mat4(1.0f), { a_Size.x, a_Size.y, 1.0f });
        DrawQuad(transform, a_Texture);
    }

    void Renderer2D::DrawQuad(const glm::mat4& a_Transform, std::shared_ptr<Texture> a_Texture)
    {
        a_Texture->Bind();

        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetMat4("u_Transform", a_Transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& a_Position, const glm::vec2& a_Size, const glm::vec4& a_Color)
    {
        DrawQuad({ a_Position.x, a_Position.y, 0.0f }, a_Size, a_Color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, const glm::vec4& a_Color)
    {
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), a_Position) * glm::scale(glm::mat4(1.0f), { a_Size.x, a_Size.y, 1.0f });
        DrawQuad(transform, a_Color);
    }

    void Renderer2D::DrawQuad(const glm::mat4& a_Transform, const glm::vec4& a_Color)
    {
        s_Data->FlatColorShader->Bind();
        s_Data->FlatColorShader->SetFloat4("u_Color", a_Color);
        s_Data->FlatColorShader->SetMat4("u_Transform", a_Transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
}
