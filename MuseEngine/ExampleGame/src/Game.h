#pragma once

#include "Core/Application.h"

class Game : public Muse::Application
{
public:
    Game() = default;
    ~Game() = default;

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float deltaTime) override;
    virtual void OnFixedUpdate() override;
    virtual void OnRender() override;

};
