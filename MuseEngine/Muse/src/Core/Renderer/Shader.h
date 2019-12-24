#pragma once

#include <string>

namespace Muse
{
    class Shader
    {
    public:
        Shader(const std::string& a_VertexSrc, const std::string& a_FragmentSrc);
        ~Shader();

        void Bind();
        void Unbind();

    private:
        uint32_t m_RendererID;
    };
}
