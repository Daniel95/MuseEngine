#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace Muse
{
    class VertexArray;
    class RendererAPI;

    class RenderCommand
    {
    public:
        static void Init();
        static void SetViewport(uint32_t a_X, uint32_t a_Y, uint32_t a_Width, uint32_t a_Height);
        static void SetClearColor(const glm::vec4& a_Color);
        static void Clear();
        static void DrawIndexed(const std::shared_ptr<VertexArray>& a_VertexArray, uint32_t a_IndexCount = 0);

    private:
        static RendererAPI* s_RendererAPI;

    };
}