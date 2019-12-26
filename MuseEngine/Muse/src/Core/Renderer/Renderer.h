#pragma once
#include "RendererAPI.h"

namespace Muse
{
    class Renderer
    {
    public:
        static void BeginScene();
        static void EndScene();
        static void Submit(const std::shared_ptr<VertexArray>& a_VertexArray);

        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    };
}
