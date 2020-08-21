#include "MusePCH.h"
#include "SubTexture2D.h"

#include "Core/Renderer/Texture.h"

namespace Muse
{
    SubTexture2D::SubTexture2D(const std::shared_ptr<Texture2D>& a_Texture, int a_X, int a_Y, glm::vec2 a_SpriteSize)
        : m_Texture(a_Texture)
    {
        glm::vec2 sheetSize = { a_Texture->GetWidth(), a_Texture->GetHeight() };

        m_TextureCoords[0] = { (a_X * a_SpriteSize.x) / sheetSize.x, (a_Y * a_SpriteSize.y) / sheetSize.y };
        m_TextureCoords[1] = { ((a_X + 1) * a_SpriteSize.x) / sheetSize.x, (a_Y * a_SpriteSize.y) / sheetSize.y };
        m_TextureCoords[2] = { ((a_X + 1) * a_SpriteSize.x) / sheetSize.x, ((a_Y + 1) * a_SpriteSize.y) / sheetSize.y };
        m_TextureCoords[3] = { (a_X * a_SpriteSize.x) / sheetSize.x, ((a_Y + 1) * a_SpriteSize.y) / sheetSize.y };
    }

    /*
    std::shared_ptr<SubTexture2D> SubTexture2D::Create(const std::shared_ptr<Texture2D>& a_Texture, int a_X, int a_Y, glm::vec2 a_SpriteSize)
    {
        float sheetWidth = 2560.0f, sheetHeight = 1664.0f;
        float spriteWidth = 128.0f, spriteHeight = 128.0f;

        glm::vec2 textureCoords[] =
        {
            { (a_X * spriteWidth) / sheetWidth, (a_Y * spriteHeight) / sheetHeight },
            { ((a_X + 1) * spriteWidth) / sheetWidth, (a_Y * spriteHeight) / sheetHeight },
            { ((a_X + 1) * spriteWidth) / sheetWidth, ((a_Y + 1) * spriteHeight) / sheetHeight },
            { (a_X * spriteWidth) / sheetWidth, ((a_Y + 1) * spriteHeight) / sheetHeight },
        };



        return std::shared_ptr<SubTexture2D>();
    }
    */

    /*
    SubTexture2D::SubTexture2D(const std::shared_ptr<Texture>& a_Texture, int a_X, int a_Y)
        : m_Texture(a_Texture)
    {





    }
    */
}