#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace Muse
{
    class Texture2D;

    class SubTexture2D
    {
    public:
        SubTexture2D(const std::shared_ptr<Texture2D>& a_Texture, int a_X, int a_Y, glm::vec2 a_SpriteSize);

        const std::shared_ptr<Texture2D> GetTexture() const { return m_Texture; }
        const glm::vec2* GetTextureCoords() const { return m_TextureCoords; }

        static std::shared_ptr<SubTexture2D> Create(const std::shared_ptr<Texture2D>& a_Texture, int a_X, int a_Y, glm::vec2 a_SpriteSize) { return std::make_shared<SubTexture2D>(a_Texture, a_X, a_Y, a_SpriteSize); }

    private:
        std::shared_ptr<Texture2D> m_Texture;
        glm::vec2 m_TextureCoords[4];

    };
}
