#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Muse
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0,
            OpenGL = 1,
        };

        virtual void SetClearColor(const glm::vec4& a_Color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& a_VertexArray) = 0;

        static API GetAPI() { return s_API; }

    private:
        static API s_API;
    };
}
