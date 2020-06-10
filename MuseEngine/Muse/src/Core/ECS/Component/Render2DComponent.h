#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Core/Renderer/Texture.h"

namespace Muse
{
	struct Render2DComponent
	{
        std::shared_ptr<Texture> texture;
        glm::vec4 color;
        glm::mat4 modelMatrix;
        float tilingFactor;
	};
}