#pragma once

#include <glm/glm.hpp>

namespace Muse
{
    enum class MaterialType
    {
        Diffuse,
        Specular,
        BlinnPhong,
        Reflective,
        Refractive,
        Checkerboard,
        NormalDebug,
    };

    struct Material
    {
        MaterialType MaterialType = MaterialType::BlinnPhong;
        glm::vec3 Color = glm::vec3(1);
        float SpecularStrength = 0.5f;
        float ReflectiveStrength = 0.5f;
        float RefractiveStrength = 0.5f;
        float Eta = 0.975f;
        float GridSize = 2;
        glm::vec3 SecondaryColor = glm::vec3(0);
        int MaxBounces = 32;
    };
}