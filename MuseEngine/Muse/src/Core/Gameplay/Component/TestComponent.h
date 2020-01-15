#pragma once

#include "Core/Gameplay/Component/Component.h"
#include <rttr/registration_friend>

namespace Muse
{
    class TestComponent : public Component
    {
        RTTR_ENABLE(Component);
        RTTR_REGISTRATION_FRIEND;

    public:
        TestComponent();
        virtual ~TestComponent();
        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("Component", cereal::base_class<Component>(this)));
            ar(
                m_Test
            );
        }

    protected:

    private:
        int m_Test = 1;

    };
}

CEREAL_REGISTER_TYPE_WITH_NAME(Muse::TestComponent, "TestComponent")
CEREAL_REGISTER_POLYMORPHIC_RELATION(Muse::Component, Muse::TestComponent)