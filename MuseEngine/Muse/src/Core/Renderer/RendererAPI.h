#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Muse
{
    class RendererAPI
    {
    public:
        virtual ~RendererAPI() = default;

        enum class API
        {
            None = 0,
            OpenGL = 1,
        };

        virtual void Init() = 0;
        virtual void SetViewport(uint32_t a_X, uint32_t a_Y, uint32_t a_Width, uint32_t a_Height) = 0;
        virtual void SetClearColor(const glm::vec4& a_Color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& a_VertexArray, uint32_t a_IndexCount = 0) = 0;

        static API GetAPI() { return s_API; }

    private:
        static API s_API;
    };
}
