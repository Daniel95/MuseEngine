#pragma once

#include "Core/Renderer/RendererAPI.h"

namespace Muse
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        void Init() override;
        virtual void SetViewport(uint32_t a_X, uint32_t a_Y, uint32_t a_Width, uint32_t a_Height) override;
        void SetClearColor(const glm::vec4& a_Color) override;
        void Clear() override;
        void DrawIndexed(const std::shared_ptr<VertexArray>& a_VertexArray, uint32_t a_IndexCount = 0) override;
    };
}