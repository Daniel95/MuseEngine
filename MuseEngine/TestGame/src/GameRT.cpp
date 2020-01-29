#include "GameRT.h"

#include "Muse.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "PlayerComponent.h"
#include "Core/Resource/ResourceTest.h"
#include "Core/Window.h"

#include "Mode.h"

#if GAME_RT
#include "EntryPoint.h"
Muse::Application* Muse::CreateApplication()
{
    return new GameRT();
}
#endif

void GameRT::OnStart()
{
    Muse::RenderCommand::Init();
    Muse::Renderer2D::Init();

    std::shared_ptr<Muse::Scene> scene = Muse::ResourceManager::Create<Muse::Scene>("New Scene");
    Muse::SceneManager::SwitchScene(scene);

    //m_ViewportTexture = Muse::ResourceManager::Create<Muse::Texture>("viewPortTexture");
    m_ViewportTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/rayman.png");
}

void GameRT::OnUpdate(float a_DeltaTime)
{
}

void GameRT::OnFixedUpdate()
{
}

void GameRT::OnRender()
{
    Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::Clear();

    Muse::Renderer2D::BeginScene(*Muse::CameraComponent::GetMain());

    /*
    const unsigned int height = GetWindow().GetHeight();
    const unsigned int width = GetWindow().GetWidth();

    std::vector<std::vector<std::array<uint32_t, 3>>> screenData;

    int size = height * width * 3;





    glm::mat4 t = glm::mat4(1);

    glm::vec3 p0 = t * glm::vec4(-1, 1, 1, 0);
    glm::vec3 p0 = t * glm::vec4(-1, 1, 1, 0);
    glm::vec3 p0 = t * glm::vec4(-1, 1, 1, 0);


    for (size_t i = 0; i < GetWindow().GetHeight(); i++)
    {
        for (size_t i = 0; i < GetWindow().GetWidth(); i++)
        {

        }
    }


    m_ViewportTexture->Bind();

    m_ViewportTexture->SetData((void*)screenData.data(), size);
    */

    const Muse::Renderer2D::QuadProperties quadProperties(
        { 0, 0, 0.1f },
        glm::vec2(1),
        45,
        //{ 0.5f, 0.5f, 0.5f, 0.5f },
        m_ViewportTexture);

    Muse::Renderer2D::DrawQuad(quadProperties);

    Muse::Renderer2D::EndScene();
}

void GameRT::OnImGuiRender()
{
}