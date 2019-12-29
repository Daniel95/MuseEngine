#pragma once
#include "Core/Gameplay/Component/Component.h"

class PlayerComponent : public Muse::Component
{
public:
    virtual void Update(float a_DeltaTime) override;

private:
    float m_MoveSpeed = 0.3f;
};
