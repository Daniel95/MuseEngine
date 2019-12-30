#pragma once

#include "Core/Renderer/RendererAPI.h"

namespace Muse
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        void Init() override;
        void SetClearColor(const glm::vec4& a_Color) override;
        void Clear() override;
        void DrawIndexed(const std::shared_ptr<VertexArray>& a_VertexArray) override;
    };
}