#include "MusePCH.h"
#include "SubTexture2D.h"

#include "Core/Renderer/Texture.h"

namespace Muse
{
    SubTexture2D::SubTexture2D(const std::shared_ptr<Texture2D>& a_Texture, const glm::vec2& a_CellCoords, const glm::vec2& a_CellSize, const glm::vec2& a_SpriteSize)
        : m_Texture(a_Texture)
    {
        glm::vec2 sheetSize = { a_Texture->GetWidth(), a_Texture->GetHeight() };

        m_TextureCoords[0] = { (a_CellCoords.x * a_CellSize.x) / sheetSize.x, (a_CellCoords.y * a_CellSize.y) / sheetSize.y };
        m_TextureCoords[1] = { ((a_CellCoords.x + a_SpriteSize.x) * a_CellSize.x) / sheetSize.x, (a_CellCoords.y * a_CellSize.y) / sheetSize.y };
        m_TextureCoords[2] = { ((a_CellCoords.x + a_SpriteSize.x) * a_CellSize.x) / sheetSize.x, ((a_CellCoords.y + a_SpriteSize.y) * a_CellSize.y) / sheetSize.y };
        m_TextureCoords[3] = { (a_CellCoords.x * a_CellSize.x) / sheetSize.x, ((a_CellCoords.y + a_SpriteSize.y) * a_CellSize.y) / sheetSize.y };
    }
}