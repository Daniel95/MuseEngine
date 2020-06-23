#pragma once

#include "Core/Application.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Entity/EntityDebugger.h"

#include "glm/glm.hpp"
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

    static int CreatePlayer(const glm::vec2& a_Position = glm::vec2(0, 0));
    static int CreateObstacle(const glm::vec2& a_Position, const glm::vec2& a_Scale = glm::vec2(1, 1));
    static int CreateEnemy(const glm::vec2& a_Position, const glm::vec2& a_Scale = glm::vec2(1, 1));
    static int CreateProjectile(Muse::TransformComponent& a_TransformComponent, float a_Speed = 5);
    static int CreateBackgroundParticle1(const glm::vec2& a_Position);
    //static int CreateBackgroundParticle2(Muse::TransformComponent& a_TransformComponent);
    //static int CreateBackgroundParticle3(Muse::TransformComponent& a_TransformComponent);
    static int CreateBackgroundParticle(Muse::TransformComponent& a_TransformComponent, const std::shared_ptr<Muse::Texture>& a_Texture, float a_ScrollingSpeed);

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float a_DeltaTime) override {}
    virtual void OnFixedUpdate() override {}
    virtual void OnRender() override;
    virtual void OnImGuiRender() override;

private:
    static std::shared_ptr<Muse::Texture> s_PlayerTexture;
    static std::shared_ptr<Muse::Texture> s_EnemyTexture;
    static std::shared_ptr<Muse::Texture> s_ProjectileTexture;
    static std::shared_ptr<Muse::Texture> s_MeteorTexture;
    static std::shared_ptr<Muse::Texture> s_Star1;
    static std::shared_ptr<Muse::Texture> s_Star2;
    static std::shared_ptr<Muse::Texture> s_Star3;

};
