#pragma once
#include "RendererAPI.h"

namespace Muse
{
    class Shader;
    class OrthographicCamera;

    class Renderer
    {
    public:
        static void BeginScene(const OrthographicCamera& a_Camera);
        static void EndScene();
        static void Submit(const std::shared_ptr<Shader>& a_Shader, const std::shared_ptr<VertexArray>& a_VertexArray);

        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* m_SceneData;
    };
}
