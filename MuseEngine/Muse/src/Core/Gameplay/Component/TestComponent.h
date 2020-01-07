#pragma once

#include "Core/Gameplay/Component/Component.h"
#include <rttr/registration_friend>

namespace Muse
{
    class TestComponent : public Component
    {
    public:
        TestComponent();
        virtual ~TestComponent();

    protected:
    private:

        RTTR_ENABLE(Component);
        RTTR_REGISTRATION_FRIEND;
    };
}