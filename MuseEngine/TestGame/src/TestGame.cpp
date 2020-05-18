#include "TestGame.h"

#include "Muse.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "PlayerComponent.h"

#include "Mode.h"

#if GAME_TEST
#include "EntryPoint.h"
Muse::Application* Muse::CreateApplication()
{
    return new TestGame();
}
#endif

void TestGame::OnStart()
{
    Muse::RenderCommand::Init();

    std::shared_ptr <Muse::Scene> scene = Muse::Scene::Create();
    Muse::ResourceManager::Add("Game2DTestScene", scene);
    Muse::SceneManager::SwitchScene(scene);

    Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });

    m_VA = Muse::VertexArray::Create();

    float quadVertices[9 * 4] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
    };

    m_VB = Muse::VertexBuffer::Create(quadVertices, 9 * 4 * sizeof(float));
    m_VB->SetLayout({
        { Muse::ShaderDataType::Float3, "a_Position" },
        { Muse::ShaderDataType::Float4, "a_Color" },
        { Muse::ShaderDataType::Float2, "a_TexCoord" }
    });
    m_VA->AddVertexBuffer(m_VB);

    uint32_t* quadIndices = new uint32_t[6]{
        0, 1, 2, 2, 3, 0
    };

    std::shared_ptr<Muse::IndexBuffer> quadIB = Muse::IndexBuffer::Create(quadIndices, 6);
    m_VA->SetIndexBuffer(quadIB);

    m_Shader = Muse::ResourceManager::Load<Muse::Shader>("assets/shaders/ColoredTexture.glsl");
    m_Shader->Bind();
}

void TestGame::OnRender()
{
    //Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::Clear();

    m_VB->Bind();
    m_Shader->Bind();
    m_VA->Bind();

    Muse::RenderCommand::DrawIndexed(m_VA, 6);
}

/*
void Game2D::OnRender()
{
    //Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::SetClearColor({ 0.9f, 0.9f, 0.9f, 1 });
    Muse::RenderCommand::Clear();

    Muse::Renderer2D::BeginScene(*Muse::CameraComponent::GetMain());

    std::shared_ptr<Muse::Scene> scene = Muse::SceneManager::GetActiveScene();

    for (auto& gameObject : Muse::SceneManager::GetActiveScene()->GetGameObjects())
    {
        auto meshComponent = gameObject->GetComponent<Muse::RenderComponent>();
        if(meshComponent != nullptr)
        {
            const Muse::Renderer2D::QuadPropertiesTransform quadPropertiesTransform(
                gameObject->GetTransform()->GetWorldModelMatrix(),
                { 0.8f, 0.2f, 0.3f, 1.0f });

            Muse::Renderer2D::DrawQuad(quadPropertiesTransform);
        }
    }

    const Muse::Renderer2D::QuadProperties quadProperties(
        { 0, 0, 0.1f },
        glm::vec2(1),
        45,
        //{ 0.5f, 0.5f, 0.5f, 0.5f },
        m_Texture);

    Muse::Renderer2D::DrawQuad(quadProperties);

    Muse::Renderer2D::EndScene();
}
*/

void TestGame::OnImGuiRender()
{
    ImGui::Begin("Test 1");
    ImGui::End();

    ImGui::Begin("Test 2");
    ImGui::End();

    ImGui::Begin("Test 3");
    ImGui::End();
}