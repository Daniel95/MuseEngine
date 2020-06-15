#pragma once

#include "glm/glm.hpp"

struct PlayerComponent 
{
    float speed = 3.5f;
    float health = 100;
    float fireCD = 0.2f;
    float fireTimer = 0;
};

struct ObstacleComponent
{
    float damage = 0;
};

struct ProjectileComponent 
{
    float speed = 3.5f;
};

struct DestroyOutOfBoundsComponent
{ 
    glm::vec2 min = glm::vec2(-5.0f, -5.0f);
    glm::vec2 max = glm::vec2(5.0f, 5.0f);
};
