#pragma once

#include "glm/glm.hpp"

struct PlayerComponent 
{
    float speed = 4.5f;
    float health = 100;
    float fireCD = 0.3f;
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
    glm::vec2 min = glm::vec2(-5.0f, -5.0f);
    glm::vec2 max = glm::vec2(5.0f, 5.0f);
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
    float speed = 3.5f;
};
