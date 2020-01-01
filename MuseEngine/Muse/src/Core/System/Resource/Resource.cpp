#include "MusePCH.h"

#include "Resource.h"

namespace Muse
{
    void Resource::Reload()
    {
    }

    RTTR_REGISTRATION
    {
        rttr::registration::class_<Resource>("Resource")
            .constructor<>()
            (
                rttr::policy::ctor::as_raw_ptr
            );
    }
}