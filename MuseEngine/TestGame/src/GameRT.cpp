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
    m_Test = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/rayman.png");

    m_Height = GetWindow().GetHeight();
    m_Width = GetWindow().GetWidth();

    m_ViewportTexture = Muse::ResourceManager::Create<Muse::Texture>("viewPortTexture", m_Width, m_Height);

    m_ScreenData.resize(m_Height * m_Width * 4);
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

    const unsigned int height = GetWindow().GetHeight();
    const unsigned int width = GetWindow().GetWidth();
    const unsigned int stride = 4;
    const uint32_t size = height * width * stride;

    if(m_Height != height || m_Width != width)
    {
        //ASSERT(false, "Resizing not supported at the moment! (because of texture resizing not existing yet...)");

        //Resize(width, height);
    }

    /*
    glm::mat4 t = glm::mat4(1);

    glm::vec3 p0 = t * glm::vec4(-1, 1, 1, 0);
    glm::vec3 p0 = t * glm::vec4(-1, 1, 1, 0);
    glm::vec3 p0 = t * glm::vec4(-1, 1, 1, 0);
    */

    glm::vec3 color = glm::vec3(1, 0.2f, 0.2f);

    int i = 0;
    for (int y = 0; y < m_Height; ++y)
    {
        for (int x = 0; x < m_Width; ++x)
        {
            m_ScreenData[i] = color.x;
            m_ScreenData[i + 1] = color.y;
            m_ScreenData[i + 2] = color.z;
            m_ScreenData[i + 3] = 1.0f;

            i += 4;
        }
    }
    m_ViewportTexture->SetDataF(&m_ScreenData[0], size);

    const Muse::Renderer2D::QuadProperties quadProperties(
        { 0, 0, 0.1f },
        glm::vec2(1),
        0,
        //{ 0.5f, 0.5f, 0.5f, 0.5f },
        m_ViewportTexture);

    Muse::Renderer2D::DrawQuad(quadProperties);

    Muse::Renderer2D::EndScene();
}

void GameRT::OnImGuiRender()
{
}

void GameRT::Resize(unsigned a_Width, unsigned a_Height)
{
    m_ScreenData.clear();

    m_ScreenData.resize(m_Height * m_Width * 4);

    //Resize texture

    m_Height = a_Height;
    m_Width = a_Width;
}
