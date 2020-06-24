#pragma once

#include "glm/glm.hpp"

struct PlayerComponent 
{
    float speed = 4.5f;
    float health = 10000000;
    float fireCD = 0.2f;
    float fireTimer = 0;
};

struct ObstacleComponent
{
    float damage = 10;
};

struct ProjectileComponent 
{
    float speed = 3.5f;
};

struct DestroyOutOfBoundsComponent
{ 
    glm::vec2 min = glm::vec2(-10.0f, -10.0f);
    glm::vec2 max = glm::vec2(10.0f, 20.0f);
};

struct EnemyComponent
{
    float speed = 3.5f;
    float health = 100;
    float fireCD = 0.25f;
    float fireTimer = 0;
};

struct ScrollingComponent
{
    float speed = 5.0f;
};

struct MeteorComponent
{
};
