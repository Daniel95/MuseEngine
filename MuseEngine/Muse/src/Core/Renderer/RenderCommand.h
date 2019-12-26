﻿#pragma once

#include <memory>

namespace Muse
{
    class VertexArray;
    class RendererAPI;

    class RenderCommand
    {
    public:
        static void SetClearColor(const glm::vec4& a_Color);
        static void Clear();
        static void DrawIndexed(const std::shared_ptr<VertexArray>& a_VertexArray);

    private:
        static RendererAPI* s_RendererAPI;

    };
}