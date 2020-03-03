#include "Game3D.h"

#include "Muse.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "PlayerComponent.h"
#include "Core/Resource/ResourceTest.h"

#include "Mode.h"

#if GAME_3D
#include "EntryPoint.h"
Muse::Application* Muse::CreateApplication()
{
    return new Game3D();
}
#endif

void Game3D::OnStart()
{
    std::shared_ptr <Muse::Scene> scene = Muse::Scene::Create();
    Muse::ResourceManager::Add("New Scene", scene);
    Muse::SceneManager::SwitchScene(scene);

    std::shared_ptr<Muse::Shader> textureShader = Muse::ResourceManager::Load<Muse::Shader>("assets/shaders/Texture.glsl");
    std::shared_ptr<Muse::Shader> vertexColorShader = Muse::ResourceManager::Load<Muse::Shader>("assets/shaders/VertexColor.glsl");
    m_FlatColorShader = Muse::ResourceManager::Load<Muse::Shader>("assets/shaders/FlatColor.glsl");

    std::shared_ptr<Muse::Texture> checkerboardTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/Checkerboard.png");
    std::shared_ptr<Muse::Texture> raymanTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/Rayman.png");

    /////////////////////////////////////////////////////////////////
    //// Checkerboard ///////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    {
        float vertices[5 * 4] =
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        };
        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

        const Muse::BufferLayout layout =
        {
            { Muse::ShaderDataType::Float3, "a_Position" },
            { Muse::ShaderDataType::Float2, "a_TexCoord" },
        };

        auto gameObject = scene->AddGameObject();
        auto renderComponent = gameObject->AddComponent<Muse::RenderComponent>();

        renderComponent->SetMesh(vertices,
            5 * 4,
            indices,
            6,
            layout);
        renderComponent->SetShader(textureShader);
        renderComponent->SetTexture(checkerboardTexture);

        textureShader->Bind();
        textureShader->SetInt("u_Texture", 0);
    }

    /////////////////////////////////////////////////////////////////
    //// Dynamic Color Square ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    {
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

        auto gameObject = scene->AddGameObject();
        auto renderComponent = gameObject->AddComponent<Muse::RenderComponent>();

        renderComponent->SetMesh(vertices,
            3 * 4,
            indices,
            6,
            layout);
        renderComponent->SetShader(m_FlatColorShader);

        gameObject->GetTransform()->SetPosition({ 1.1f, 0, 0 });
    }

    /////////////////////////////////////////////////////////////////
    //// Triangle ///////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////    
    {
        float vertices[3 * 7]
        {
            -0.5f, -0.5f, -0.0f, 0.9f, 0.9f, 0.1f, 0.6f,
            0.5f, -0.5f, -0.0f, 0.1f, 0.6f, 0.2f, 0.8f,
            0.0f, 0.5f, -0.0f, 0.2, 0.3, 0.8, 1.0,
        };
        uint32_t indices[3] = { 0, 1, 2 };

        const Muse::BufferLayout layout =
        {
            { Muse::ShaderDataType::Float3, "a_Position" },
            { Muse::ShaderDataType::Float4, "a_Color" }
        };

        auto gameObject = scene->AddGameObject();
        auto renderComponent = gameObject->AddComponent<Muse::RenderComponent>();

        renderComponent->SetMesh(vertices,
            3 * 7,
            indices,
            3,
            layout);

        renderComponent->SetShader(vertexColorShader);


        gameObject->GetTransform()->SetPosition({ -1, 0, 0 });
    }

    /////////////////////////////////////////////////////////////////
    //// Player Textured Square /////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    {
        float vertices[5 * 4] =
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        };
        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

        const Muse::BufferLayout layout =
        {
            { Muse::ShaderDataType::Float3, "a_Position" },
            { Muse::ShaderDataType::Float2, "a_TexCoord" },
        };

        auto gameObject = scene->AddGameObject();
        gameObject->AddComponent<PlayerComponent>();
        auto renderComponent = gameObject->AddComponent<Muse::RenderComponent>();

        renderComponent->SetMesh(vertices,
            5 * 4,
            indices,
            6,
            layout);
        renderComponent->SetShader(textureShader);
        renderComponent->SetTexture(raymanTexture);

        textureShader->Bind();
        textureShader->SetInt("u_Texture", 0);

        gameObject->GetTransform()->SetPosition({ 0, 0, 1 });
    }
}

void Game3D::OnRender()
{
    Muse::Renderer::Init();

    Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::Clear();

    Muse::Renderer::BeginScene(*Muse::CameraComponent::GetMain());

    std::shared_ptr<Muse::Shader> flatColorShader = Muse::ResourceManager::Get<Muse::Shader>("assets/shaders/FlatColor.glsl");
    flatColorShader->Bind();
    flatColorShader->SetFloat4("u_Color", m_FlatShaderColor);

    for (auto& gameObject : Muse::SceneManager::GetActiveScene()->GetGameObjects())
    {
        auto meshComponent = gameObject->GetComponent<Muse::RenderComponent>();

        if (meshComponent != nullptr)
        {
            if (meshComponent->GetTexture() != nullptr)
            {
                meshComponent->GetTexture()->Bind();
            }
            Muse::Renderer::Submit(meshComponent->GetShader(), meshComponent->GetVA(), gameObject->GetTransform()->GetModelMatrix());
        }
    }

    Muse::Renderer::EndScene();
}

void Game3D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_FlatShaderColor));
    ImGui::End();
}