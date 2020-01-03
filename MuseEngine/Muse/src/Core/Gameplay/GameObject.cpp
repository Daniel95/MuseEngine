#include "MusePCH.h"

#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Scene/Scene.h"

namespace Muse 
{
    GameObject::~GameObject()
    {
        for (Component* component : m_Components)
        {
            delete component;
        }
    }

    void GameObject::Init(Scene& a_Scene)
    {
        m_Scene = &a_Scene;

        if (!HasComponent<TransformComponent>())
        {
            AddComponent<TransformComponent>();
        }
    }

    void GameObject::Update(float a_DeltaTime)
    {
        for (Component* component : m_Components)
        {
			if (component->IsEnabled())
			{
				component->Update(a_DeltaTime);
			}
        }
    }

    void GameObject::FixedUpdate(float a_TimeStep)
    {
        for (Component* component : m_Components)
        {
            if (component->IsEnabled())
            {
                component->FixedUpdate();
            }
        }
    }

    void GameObject::Destroy()
    {
        if (!m_Destroyed)
        {
            m_Scene->RemoveGameObject(*this);
            m_Destroyed = true;
        }
    }

    void GameObject::Enable()
    {
        for (Component* component : m_Components)
        {
            component->Enable();
        }
    }

    void GameObject::Disable()
    {
        for (Component* component : m_Components)
        {
            component->Disable();
        }
    }

    Scene* GameObject::GetScene() const
    {
        return m_Scene;
    }

    TransformComponent* GameObject::GetTransform() const
    {
        return GetComponent<TransformComponent>();
    }

    void GameObject::SetComponents(const std::vector<Component*>& a_Components)
    {
        m_Components = a_Components;
    }

    const std::vector<Component*>& GameObject::GetComponents()
    {
        return m_Components;
    }

    RTTR_REGISTRATION
    {
        rttr::registration::class_<GameObject>("GameObject")
            .constructor<>()
            (
                rttr::policy::ctor::as_raw_ptr
            )
            .property("Components", &GameObject::GetComponents, &GameObject::SetComponents);
    }
}
