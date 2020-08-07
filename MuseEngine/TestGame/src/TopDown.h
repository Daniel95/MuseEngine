#pragma once

#include "Core/Application.h"

#include "glm/glm.hpp"
#include <vector>

namespace Muse
{
    class Texture;
}

class TopDown : public Muse::Application
{
public:
    TopDown() = default;
    virtual ~TopDown() = default;

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float a_DeltaTime) override {}
    virtual void OnFixedUpdate() override {}
    virtual void OnRender() override;
    virtual void OnImGuiRender() override;

private:
    std::shared_ptr<Muse::Texture> m_SpriteSheet;
    std::shared_ptr<Muse::Texture> m_EnemyTexture;

};
