#include "MusePCH.h"

#include "Core/Application.h"

#include "Core/System/ResourceSystem.h"
#include "Core/System/SceneSystem.h"
#include "Core/System/SoundSystem.h"
#include "Core/System/Manager/SystemManager.h"
#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"
#include "Core/Event/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Core/Input/Input.h"
#include "Core/Input/MouseButtonCodes.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Renderer/Shader.h"
#include "Renderer/Buffer/VertexBuffer.h"
#include "Renderer/Buffer/IndexBuffer.h"

#include <glad/glad.h>
#include "Renderer/Buffer/BufferLayout.h"

namespace Muse
{
    Application* Application::s_Instance = nullptr;

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType a_ShaderDataType)
    {
        switch (a_ShaderDataType)
        {
            case ShaderDataType::Float:    return GL_FLOAT;
            case ShaderDataType::Float2:   return GL_FLOAT;
            case ShaderDataType::Float3:   return GL_FLOAT;
            case ShaderDataType::Float4:   return GL_FLOAT;
            case ShaderDataType::Mat3:     return GL_FLOAT;
            case ShaderDataType::Mat4:     return GL_FLOAT;
            case ShaderDataType::Int:      return GL_INT;
            case ShaderDataType::Int2:     return GL_INT;
            case ShaderDataType::Int3:     return GL_INT;
            case ShaderDataType::Int4:     return GL_INT;
            case ShaderDataType::Bool:     return GL_BOOL;
        }

        ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    Application::Application()
    {
        ASSERT(!s_Instance, "A instance of Application already exists!");
        s_Instance = this;


        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->WindowCloseEvent.Subscribe(this, std::bind(&Application::OnWindowCloseEvent, this));
        m_Window->WindowResizeEvent.Subscribe(this, std::bind(&Application::OnWindowResizeEvent, this, std::placeholders::_1, std::placeholders::_2));
        m_Window->KeyPressedEvent.Subscribe(this, std::bind(&Application::OnWindowResizeEvent, this, std::placeholders::_1, std::placeholders::_2));
        m_Window->KeyReleasedEvent.Subscribe(this, std::bind(&Application::OnKeyReleasedEvent, this, std::placeholders::_1));
        m_Window->MouseButtonPressedEvent.Subscribe(this, std::bind(&Application::OnMouseButtonPressedEvent, this, std::placeholders::_1));
        m_Window->MouseButtonReleasedEvent.Subscribe(this, std::bind(&Application::OnMouseButtonReleasedEvent, this, std::placeholders::_1));
        m_Window->MouseScrolledEvent.Subscribe(this, std::bind(&Application::OnMouseScrolledEvent, this, std::placeholders::_1, std::placeholders::_2));
        m_Window->MouseMovedEvent.Subscribe(this, std::bind(&Application::OnMouseMovedEvent, this, std::placeholders::_1, std::placeholders::_2));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        m_SystemManager = new SystemManager();
        m_SystemManager->CreateSystem<ResourceSystem>();
        m_SystemManager->CreateSystem<SoundSystem>();
        m_SystemManager->CreateSystem<SceneSystem>(*this);

        // Vertex Array
        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        //Triangle vertices
        float vertices[3 * 7] =
        {
            -0.5f, -0.5f, -0.0f, 0.9f, 0.9f, 0.1f, 0.6f,
            0.5f, -0.5f, -0.0f, 0.1f, 0.6f, 0.2f, 0.8f,
            0.0f, 0.5f, -0.0f, 0.7f, 0.8f, 0.2f, 0.1f,
        };
        uint32_t indices[3] = { 0, 1, 2 };

        m_VertexBuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));
        m_VertexBuffer->Bind();

        {
            const BufferLayout layout =
            {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color" }
            };
            m_VertexBuffer->SetLayout(layout);
        }

        uint32_t index = 0;
        const auto& layout = m_VertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            //Location, Amount of type, Type, normalized, stride (vertex size), pointer
            glVertexAttribPointer(index,
                element.GetNumberCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)element.Offset);
            index++;
        }

        uint32_t count = sizeof(indices) / sizeof(uint32_t);
        m_IndexBuffer = std::unique_ptr<IndexBuffer>(IndexBuffer::Create(indices, count));

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec3 v_Position;
            out vec4 v_Color;
            
            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";


        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";

        m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
    }

    Application::~Application()
    {
        m_Window->WindowCloseEvent.Unsubscribe(this);
        m_Window->WindowResizeEvent.Unsubscribe(this);
        m_Window->KeyPressedEvent.Unsubscribe(this);
        m_Window->KeyReleasedEvent.Unsubscribe(this);
        m_Window->MouseButtonPressedEvent.Unsubscribe(this);
        m_Window->MouseButtonReleasedEvent.Unsubscribe(this);
        m_Window->MouseScrolledEvent.Unsubscribe(this);
        m_Window->MouseMovedEvent.Unsubscribe(this);

        delete m_SystemManager;
    }

    void Application::Start()
    {
        OnStart();

        while (m_Running)
        {
            Update();
            FixedUpdate();
            Render();
        }
    }

    void Application::Update()
    {
        for (Layer* layer : m_LayerStack)
        {
            layer->OnUpdate(0.16f);
        }

        m_SystemManager->UpdateSystems(0);
        OnUpdate(0.016f);
    }

    void Application::FixedUpdate()
    {
        OnFixedUpdate();
    }

    void Application::Render()
    {
        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack)
        {
            layer->OnImGuiRender();
        }
        m_ImGuiLayer->End();

        m_Window->OnUpdate();

        OnRender();
        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_Shader->Bind();
        glBindVertexArray(m_VertexArray);
        glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnWindowCloseEvent()
    {
        m_Running = false;
    }

    void Application::OnWindowResizeEvent(int a_Width, int a_Height)
    {
        //LOG_ENGINE_INFO("OnWindowResize: {0}, {1}", a_Width, a_Height);
    }

    void Application::OnKeyPressedEvent(int a_KeyCode, int a_RepeatCount)
    {
        //LOG_ENGINE_INFO("OnKeyPressed: {0}, {1}", a_KeyCode, a_RepeatCount);
    }

    void Application::OnKeyReleasedEvent(int a_KeyCode)
    {
        //LOG_ENGINE_INFO("OnKeyReleased: {0}", a_KeyCode);
    }

    void Application::OnMouseButtonPressedEvent(int a_Button)
    {
        //LOG_ENGINE_INFO("OnMouseButtonPressed: {0}", a_Button);
    }

    void Application::OnMouseButtonReleasedEvent(int a_Button)
    {
        //LOG_ENGINE_INFO("OnMouseButtonReleased: {0}", a_Button);
    }

    void Application::OnMouseScrolledEvent(float a_XOffset, float a_YOffset)
    {
        //LOG_ENGINE_INFO("OnMouseScrolled: {0}, {1}", a_XOffset, a_YOffset);
    }

    void Application::OnMouseMovedEvent(float a_X, float a_Y)
    {
        //LOG_ENGINE_INFO("OnMouseMoved: {0}, {1}", a_X, a_Y);
    }
}
