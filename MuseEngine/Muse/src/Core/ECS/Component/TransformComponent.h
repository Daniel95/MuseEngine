#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

#include "Core/ECS/Entity/Entity.h"

namespace Muse
{
    struct TransformComponent
    {
        glm::vec3 localPosition = glm::vec3(0);
        glm::vec3 localScale = glm::vec3(1);
        glm::vec3 localRotation = glm::vec3(0);

        glm::mat4 worldModelMatrix = glm::identity<glm::mat4>();

        std::vector<Entity> children;
        Entity* parent = nullptr;
        bool dirty = true;
    };
}