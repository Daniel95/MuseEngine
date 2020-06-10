#pragma once

#include <rttr/registration>
#include <rttr/type>
#include <rttr/registration_friend.h>

#include <memory>

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

namespace Muse
{
    class GameObject;
    class BoxCollider2D;
    class TransformComponentOld;

    class Component
    {
        RTTR_ENABLE()
        RTTR_REGISTRATION_FRIEND

    public:
        Component() = default;
        virtual ~Component() = default;

        void Init(std::shared_ptr<GameObject> a_GameObject);
        void Update(float a_DeltaTime) { OnUpdate(a_DeltaTime); }
        void FixedUpdate() { OnFixedUpdate(); }
		void Enable();
        void Disable();

        std::shared_ptr<GameObject> GetGameObject() const { return m_GameObject; }
        std::shared_ptr<TransformComponentOld> GetTransform() const;
		bool IsEnabled() const { return m_isEnabled; };
        //template<typename T>
        //T* GetComponent() { return m_GameObject->GetComponent<T>(); }

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(m_isEnabled);
        }
	protected:
        virtual void OnUpdate(float a_DeltaTime) {}
        virtual void OnInit() {}
        virtual void OnFixedUpdate() {}
		virtual void OnEnable() {}
		virtual void OnDisable() {}

    private:
        std::shared_ptr<GameObject> m_GameObject = nullptr;
		bool m_isEnabled = true;
        bool m_Initialized = false;

    };
}

CEREAL_REGISTER_TYPE_WITH_NAME(Muse::Component, "Component")
