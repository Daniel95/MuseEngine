#pragma once

struct PlayerComponent 
{
    float speed = 3.5f;
    float health = 100;
};

struct ObstacleComponent
{
    float damage = 0;
};

struct MoveForwardComponent 
{
    float speed = 3.5f;
};

struct DeleteOutOfBoundsComponent { };
