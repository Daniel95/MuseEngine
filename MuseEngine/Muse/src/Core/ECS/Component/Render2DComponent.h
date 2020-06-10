#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Core/Renderer/Texture.h"

namespace Muse
{
	struct Render2DComponent
	{
        std::shared_ptr<Texture> texture = nullptr;
        glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
        glm::mat4 modelMatrix = glm::identity<glm::mat4>();
        float tilingFactor = 1;
	};
}