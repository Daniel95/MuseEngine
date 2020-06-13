#pragma once

namespace Muse
{
    struct BoundingBox
    {
        float minX;
        float maxX;
        float minY;
        float maxY;
    };

    static class Collision2DHelper
    {
    public:
        static bool AABBCheck(const BoundingBox& a_Collider1, const BoundingBox& a_Collider2);

    };
}