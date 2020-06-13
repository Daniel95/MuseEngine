#pragma once

#include <vector>

namespace Muse
{
    class Collision2D
    {
    public:
        struct BoundingBox
        {
            BoundingBox(const glm::vec2& a_Position, const glm::vec2& a_Size)
            {
                glm::vec2 a_HalfSize = glm::vec2(a_Size.x / 2, a_Size.y / 2);

                minX = a_Position.x - a_HalfSize.x;
                maxX = a_Position.x + a_HalfSize.x;
                minY = a_Position.y - a_HalfSize.y;
                maxY = a_Position.y + a_HalfSize.y;
            }

            float minX;
            float maxX;
            float minY;
            float maxY;
        };

        static void GetEntityHits(const std::vector<int>& entities, std::vector<std::pair<int, int>>& hits);
        static bool AABBCheck(const BoundingBox& a_Collider1, const BoundingBox& a_Collider2);

    };
}
