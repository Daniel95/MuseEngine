#pragma once

#include <glm/glm.hpp>

namespace Muse
{
    struct Material;

    class RendererPT
    {
    public:
        static glm::vec3 GetColor(const Material& a_Material);

    };
}
