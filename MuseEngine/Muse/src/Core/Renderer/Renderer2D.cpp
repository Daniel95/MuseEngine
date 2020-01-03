#include "MusePCH.h"
#include "Renderer2D.h"

#include "Core/Gameplay/Component/CameraComponent.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Buffer/BufferLayout.h"

#include <memory>
#include "Core/Resource/ResourceManager.h"

namespace Muse
{
    struct Renderer2DStorage
    {
        std::shared_ptr<VertexArray> QuadVertexArray;
        std::shared_ptr<Shader> FlatColorShader;
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

        s_Data->FlatColorShader = ResourceManager::Get<Shader>("assets/shaders/FlatColor.glsl");
    }

    void Renderer2D::ShutDown()
    {
        delete s_Data;
    }

    void Renderer2D::BeginScene(const CameraComponent& a_OrthographicCamera)
    {
        s_Data->FlatColorShader->Bind();
        s_Data->FlatColorShader->SetMat4("u_ViewProjection", a_OrthographicCamera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene()
    {
    }

    void Renderer2D::DrawQuad(const glm::vec2& a_Position, const glm::vec2 a_Size, const glm::vec4 a_Color)
    {
    }

    void Renderer2D::DrawQuad(const glm::vec3& a_Position, const glm::vec2 a_Size, const glm::vec4 a_Color)
    {

    }
}
