#include "MusePCH.h"
#include "Renderer2D.h"

#include "Core/Gameplay/Component/CameraComponent.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Buffer/BufferLayout.h"
#include "Core/Gameplay/Component/RenderComponent.h"

namespace Muse
{
    struct Renderer2DStorage
    {
        std::shared_ptr<VertexArray> QuadVertexArray;
        std::shared_ptr<Shader> FlatColorShader;
    };

    static Renderer2DStorage s_Data;
    
    void Renderer2D::Init()
    {

        /*
        float vertices[3 * 4] =
        {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
        };
        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

        const Muse::BufferLayout layout =
        {
            { Muse::ShaderDataType::Float3, "a_Position" },
        };

        s_Data.
        */
        /*
        renderComponent.SetMesh(vertices,
            3 * 4,  
            indices,
            6,
            layout);
        renderComponent.SetShader(m_FlatColorShader);

        gameObject.GetTransform()->SetPosition({ 1.1f, 0, 0 });
        */

    }

    void Renderer2D::ShutDown()
    {
    }

    void Renderer2D::BeginScene(const CameraComponent& a_OrthographicCamera)
    {
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
