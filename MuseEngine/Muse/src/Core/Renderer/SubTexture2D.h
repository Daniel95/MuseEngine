#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace Muse
{
    class Texture2D;

    class SubTexture2D
    {
    public:
        SubTexture2D(const std::shared_ptr<Texture2D>& a_Texture,
            const glm::vec2& a_CellCoords, 
            const glm::vec2& a_CellSize, 
            const glm::vec2& a_SpriteSize = { 1, 1 });

        const std::shared_ptr<Texture2D> GetTexture() const { return m_Texture; }
        const glm::vec2* GetTextureCoords() const { return m_TextureCoords; }

        static std::shared_ptr<SubTexture2D> Create(const std::shared_ptr<Texture2D>& a_Texture, 
            const glm::vec2& a_CellCoords, 
            const glm::vec2& a_CellSize, 
            const glm::vec2& a_SpriteSize = { 1, 1 }) { return std::make_shared<SubTexture2D>(a_Texture, a_CellCoords, a_CellSize, a_SpriteSize); }

    private:
        std::shared_ptr<Texture2D> m_Texture = nullptr;
        glm::vec2 m_TextureCoords[4];

    };
}
