#pragma once

#include "Core/Application.h"

#include "glm/vec3.hpp"
#include <vector>

namespace Muse
{
    class Texture;
}

class BulletHell : public Muse::Application
{
public:
    BulletHell() = default;
    virtual ~BulletHell() = default;

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float a_DeltaTime) override {}
    virtual void OnFixedUpdate() override {}
    virtual void OnRender() override;
    virtual void OnImGuiRender() override;

private:
    std::shared_ptr<Muse::Texture> m_RaymanTexture;
    std::shared_ptr<Muse::Texture> m_CheckerboardTexture;

    int CreatePlayer(const glm::vec2& a_Position);
    int CreateObstacle(const glm::vec2& a_Position, const glm::vec2& a_Scale = glm::vec2(1, 1));

};
