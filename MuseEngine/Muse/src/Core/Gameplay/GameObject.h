#pragma once

#include "Core/Gameplay/Component/Component.h"
#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Gameplay/Component/CameraComponent.h"

#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"

#include <rttr/registration>
#include <rttr/registration_friend>

#include <vector>
#include <memory>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

namespace Muse 
{
    class Scene;
    class TransformComponent;
    class SystemManager;
	class GameObject : public std::enable_shared_from_this<GameObject>
	{
        RTTR_ENABLE()
        RTTR_REGISTRATION_FRIEND

	public:
        GameObject() = default;
		virtual ~GameObject();

        void Init(Scene& a_Scene);
        void Update(float a_DeltaTime);
        void FixedUpdate(float a_TimeStep);
        void Enable();
        void Disable();

        template <class T>
        bool HasComponent();
        template <class T>
        std::shared_ptr<T> AddComponent();
        template <class T>
        void RemoveComponent();
        template <class T>
        std::shared_ptr<T> GetComponent() const;
        template <class T>
        bool TryGetComponent(std::shared_ptr<const T> a_Component) const;
        Scene* GetScene() const { return  m_Scene; }
        std::shared_ptr<TransformComponent> GetTransform() const { return m_TransformComponent; }

        const std::vector<std::shared_ptr<Component>>& GetComponents() const { return m_Components;  }
        void Destroy();

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(
                cereal::make_nvp("m_Components", m_Components)
            );
        }
	private:
        std::vector<std::shared_ptr<Component>> m_Components;
        Scene* m_Scene;
        bool m_Destroyed = false;
        std::shared_ptr<TransformComponent> m_TransformComponent;

	};

    template<class T>
    bool GameObject::HasComponent()
    {
        static_assert(std::is_base_of<Component, T>::value, "Type must derive from Component");
        
        return GetComponent<T>() != nullptr;
    }

    template<class T>
    std::shared_ptr<T> GameObject::AddComponent()
    {
        static_assert(std::is_base_of<Component, T>::value, "Type must derive from Component");
        ASSERT_ENGINE(!HasComponent<T>(), "Component already exists on this gameobject!");

        std::shared_ptr<T> type = std::make_shared<T>();

        std::shared_ptr<Component> component = std::dynamic_pointer_cast<Component>(type);
        component->Init(shared_from_this());
        m_Components.push_back(component);

        return type;
    }

    template<class T>
    void GameObject::RemoveComponent()
    {
        static_assert(std::is_base_of<Component, T>::value, "Type must derive from Component");

        std::shared_ptr<T> componentOfType = GetComponent<T>();

        ASSERT_ENGINE(componentOfType != nullptr, "Can't remove component because this component doesn't exists.");

        const std::shared_ptr<Component> component = std::dynamic_pointer_cast<Component>(componentOfType);

        m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), component), m_Components.end());
    }

    template<class T>
    std::shared_ptr<T> GameObject::GetComponent() const
    {
        static_assert(std::is_base_of<Component, T>::value, "Type must derive from Component");

        std::shared_ptr<T> componentOfType = nullptr;

        for (const auto& component : m_Components)
        {
            componentOfType = std::dynamic_pointer_cast<T>(component);

            if (componentOfType != nullptr)
            {
                break;
            }
        }

        return componentOfType;
    }

    template <class T>
    bool GameObject::TryGetComponent(std::shared_ptr<const T> a_Component) const
    {
        static_assert(std::is_base_of<Component, T>::value, "Type must derive from Component");

        a_Component = GetComponent<T>();

        return a_Component != nullptr;
    }
}

CEREAL_REGISTER_TYPE_WITH_NAME(Muse::GameObject, "GameObject")
