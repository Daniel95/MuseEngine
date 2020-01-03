#pragma once
#include "RendererAPI.h"

namespace Muse
{
    class CameraComponent;
    class Shader;
    class OrthographicCamera;

    class Renderer
    {
    public:
        static void Init();
        static void OnWindowResize(uint32_t a_Width, uint32_t a_Height);
        static void BeginScene(const CameraComponent& a_Camera);
        static void EndScene();
        static void Submit(const std::shared_ptr<Shader>& a_Shader, const std::shared_ptr<VertexArray>& a_VertexArray, const glm::mat4& a_Transform = glm::mat4(1.0f));

        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* s_SceneData;
    };
}
