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
            ar(
            );
        }
    protected:
    private:


    };
}