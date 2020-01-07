#include "MusePCH.h"
#include "TestComponent.h"

namespace Muse 
{
    TestComponent::TestComponent() 
    {
    }

    TestComponent::~TestComponent()
    {
    }
}

RTTR_REGISTRATION
{
    rttr::registration::class_<Muse::TestComponent>("TestComponent")
        .constructor<>()
        (
            rttr::policy::ctor::as_raw_ptr
        );
//.property("Enabled", &Muse::Component::m_isEnabled);
}