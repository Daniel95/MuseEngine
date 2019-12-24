#include "MusePCH.h"
#include "BufferLayout.h"

namespace Muse
{
    void BufferLayout::CalculateOffsetAndStride()
    {
        uint32_t offset = 0;
        m_Stride = 0;

        for (auto& element : m_Elements)
        {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
    }
}
